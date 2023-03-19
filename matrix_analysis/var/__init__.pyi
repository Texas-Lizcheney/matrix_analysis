import typing

__support_num = typing.Union[None, int, float, complex, variable]
__support_component = typing.Union[int, float]


def set_print_precision(precision: int) -> None:
    ...


def set_arg_format(format: str) -> None:
    ...


class variable:
    real: float
    imag: float
    rec: typing.Union[typing.Tuple[float, float], None]
    length: float
    arg: float
    pol: typing.Union[typing.Tuple[float, float], None]
    is_arbitrary: bool

    @typing.overload
    def __init__(self) -> variable:
        ...

    @typing.overload
    def __init__(self, num: __support_num) -> variable:
        ...

    @typing.overload
    def __init__(self, real: __support_component,
                 imag: __support_component) -> variable:
        ...

    def __repr__(self) -> str:
        ...

    def __str__(self) -> str:
        ...

    def __add__(self, other: __support_num) -> variable:
        ...

    def __radd__(self, other: __support_num) -> variable:
        ...

    def __sub__(self, other: __support_num) -> variable:
        ...

    def __rsub__(self, other: __support_num) -> variable:
        ...

    def __mul__(self, other: __support_num) -> variable:
        ...

    def __rmul__(self, other: __support_num) -> variable:
        ...

    def __mod__(self, other: __support_num) -> variable:
        ...

    def __rmod__(self, other: __support_num) -> variable:
        ...

    def __divmod__(self, other: __support_num) -> typing.Union[variable, variable]:
        ...

    def __rdivmod__(self, other: __support_num) -> typing.Union[variable, variable]:
        ...

    def __pow__(self, other: __support_num, mod: __support_component) -> variable:
        ...

    def __rpow__(self, other: __support_num, mod: __support_component) -> variable:
        ...

    def __neg__(self) -> variable:
        ...

    def __pos__(self) -> variable:
        ...

    def __abs__(self) -> float:
        ...

    def __bool__(self) -> bool:
        ...

    def __floordiv__(self, other: __support_num) -> variable:
        ...

    def __truediv__(self, other: __support_num) -> variable:
        ...


class Undefined(Exception):
    ...
