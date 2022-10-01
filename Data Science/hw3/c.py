import functools
import signal


class TimeoutException(RuntimeError):
    def __init__(self, message=None):
        super().__init__(message)


def signal_handler(sig, frame):
    raise TimeoutException("Timed out")


def timeout(seconds=0.1):

    def decorator(func):
        if seconds is None or seconds <= 0.0:
            return func

        @functools.wraps(func)
        def wrapper(*args, **kwargs):
            signal.signal(signal.SIGALRM, signal_handler)
            signal.setitimer(signal.ITIMER_REAL, seconds)
            try:
                res = func(*args, **kwargs)
            except Exception as e:
                signal.setitimer(signal.ITIMER_REAL, 0)
                signal.signal(signal.SIGALRM, signal.SIG_DFL)
                raise e
            signal.setitimer(signal.ITIMER_REAL, 0)
            signal.signal(signal.SIGALRM, signal.SIG_DFL)
            return res
        return wrapper
    return decorator
