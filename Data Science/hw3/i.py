from copy import copy as shallowcopy
from copy import deepcopy


class FragileDict:

    _data = None
    _lock = True

    def __init__(self, data={}, lock=True):
        self._data = deepcopy(data)
        self._lock = lock

    def __contains__(self, key):
        return key in self._data

    def __getitem__(self, key):
        if key in self._data:
            if self._lock:
                return deepcopy(self._data[key])
            else:
                self.__keys.append(key)
                return self._data[key]
        raise KeyError(key)

    def __setitem__(self, key, val):
        if self._lock:
            raise RuntimeError("Protected state")
        else:
            self._data[key] = deepcopy(val)

    def __enter__(self):
        if self._data is None:
            self._data = {}
        self.__keys = []
        self.__copied = deepcopy(self._data)
        self._lock = False
        return self

    def __exit__(self, exc_type, exc_value, exc_traceback):
        self._lock = True
        if exc_type is not None or exc_value is not None or exc_traceback is not None:
            print("Exception has been suppressed.")
            self._data = self.__copied
        else:
            for key in self.__keys:
                self._data[key] = deepcopy(self._data[key])
        del self.__keys
        del self.__copied
        return True
