import re, os, pty, subprocess, termios, fcntl, struct, select, time, signal
master, slave = pty.openpty()
fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack('HHHH', 24, 80, 0, 0))
before = termios.tcgetattr(slave)
p = subprocess.Popen(['./build/muscli', 'tests/test-tone.mp3'], stdin=slave, stdout=slave, stderr=slave)
def read_for(seconds):
    data = b''
    end = time.monotonic() + seconds
    while time.monotonic() < end:
        if select.select([master], [], [], 0.05)[0]:
            data += os.read(master, 65536)
    return re.sub(rb"\x1b\[[0-?]*[ -/]*[@-~]", b"", data)
try:
    out = read_for(0.7)
    if p.poll() is not None:
        raise RuntimeError(out.decode(errors='replace'))
    os.write(master, b' ')
    out += read_for(0.3)
    assert b'Paused' in out, 'pause not displayed'
    os.write(master, b' ')
    out += read_for(4.5)
    assert b'Finished' in out, 'finish not displayed'
    os.write(master, b' ')
    restarted = read_for(0.3)
    assert b'Playing' in restarted, 'restart not displayed'
    fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack('HHHH', 5, 20, 0, 0))
    read_for(0.2)
    os.write(master, b'q')
    p.wait(timeout=3)
    assert p.returncode == 0
    assert before == termios.tcgetattr(slave), 'terminal settings changed'
    print('PASS: pause, resume, finish, restart, resize, q, terminal restoration')
finally:
    if p.poll() is None: p.kill(); p.wait()
    os.close(master); os.close(slave)

# Signals and invalid files must also restore the saved terminal settings.
import tempfile
import shutil
with tempfile.TemporaryDirectory() as directory:
    bad = os.path.join(directory, 'corrupt file.mp3')
    with open(bad, 'wb') as output:
        output.write(b'not audio')
    spaced = os.path.join(directory, 'test tone.mp3')
    shutil.copyfile('tests/test-tone.mp3', spaced)
    for audio, shutdown in [(spaced, signal.SIGINT),
                            ('tests/test-tone.mp3', signal.SIGTERM),
                            (bad, None)]:
        master, slave = pty.openpty()
        fcntl.ioctl(slave, termios.TIOCSWINSZ, struct.pack('HHHH', 24, 80, 0, 0))
        before = termios.tcgetattr(slave)
        process = subprocess.Popen(['./build/muscli', audio], stdin=slave,
                                   stdout=slave, stderr=slave)
        try:
            read_for(0.7)
            if shutdown:
                process.send_signal(shutdown)
                read_for(0.5)
            process.wait(timeout=3)
            assert process.returncode == (0 if shutdown else 1)
            assert before == termios.tcgetattr(slave), 'terminal settings changed'
        finally:
            if process.poll() is None:
                process.kill()
                process.wait()
            os.close(master)
            os.close(slave)
print('PASS: Ctrl+C, SIGTERM, corrupt file, terminal restoration')

for args in [[], ['tests/missing.mp3']]:
    result = subprocess.run(['./build/muscli', *args], capture_output=True)
    assert result.returncode == 1 and result.stderr
print('PASS: usage, missing file, path containing spaces')
