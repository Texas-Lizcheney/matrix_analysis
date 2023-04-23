import typing
import numpy
from ..var import variable
from ..__init__ import Unsure

__support_num = typing.Union[Unsure, int, float, complex, variable]
__support_component = typing.Union[int, float]


def set_fastprint(value: bool) -> None:
    ...


def set_printarea(*, left: int, right: int, up: int, down: int) -> None:
    ...


class matrix:
    rows: int
    cols: int
    total: int
    shape: tuple[int, int]

    @typing.overload
    def __init__(self, rows: int, cols: int, *, fill: __support_num) -> None:
        ...

    @typing.overload
    def __init__(self, matrix: list[list[__support_num]], *, fill: __support_num) -> None:
        ...

    @typing.overload
    def __init__(self, matrix: list[tuple[int, int, __support_num]], *, fill: __support_num) -> None:
        ...

    @typing.overload
    def __init__(self, matrix: numpy.ndarray) -> None:
        ...

    def __repr__(self) -> str:
        ...

    def __str__(self) -> str:
        ...

    def __len__(self) -> int:
        ...

    @typing.overload
    def __getitem__(self, index: tuple[int | slice, int | slice]) -> variable | matrix:
        ...

    @typing.overload
    def __getitem__(self, index: ellipsis) -> matrix:
        ...

    @typing.overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: __support_num) -> matrix:
        ...

    @typing.overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: list[list[__support_num]]) -> matrix:
        ...

    @typing.overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: list[tuple[int, int, __support_num]]) -> matrix:
        ...

    @typing.overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: numpy.ndarray) -> matrix:
        ...

    def __add__(self, other: matrix) -> matrix:
        ...

    def __radd__(self, other: matrix) -> matrix:
        ...

    def __sub__(self, other: matrix) -> matrix:
        ...

    def __rsub__(self, other: matrix) -> matrix:
        ...

    def __mul__(self, other: __support_num) -> matrix:
        ...

    def __rmul__(self, other: __support_num) -> matrix:
        ...

    def __mod__(self, other: __support_num) -> matrix:
        ...

    def __rmod__(self, other: __support_num) -> matrix:
        ...

    def __divmod__(self, other: __support_num) -> tuple[matrix, matrix]:
        ...

    def __rdivmod__(self, other: __support_num) -> tuple[matrix, matrix]:
        ...

    def __floordiv__(self, other: __support_num) -> matrix:
        ...

    def __rfloordiv__(self, other: __support_num) -> matrix:
        ...

    def __truediv__(self, other: __support_num) -> matrix:
        ...

    def __rtruediv__(self, other: __support_num) -> matrix:
        ...

    def __matmul__(self, other: matrix) -> matrix:
        ...

    def __rmatmul__(self, other: matrix) -> matrix:
        ...
