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


class Undefined(Exception):
    ...
