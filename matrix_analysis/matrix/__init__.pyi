from typing import overload, TypeVar, Self
from numpy import ndarray
from ..var import variable, errordouble
from ..__init__ import unsure, Unsure

__support_num = unsure | int | float | complex | variable
__support_num_var = TypeVar(
    "__support_num_var", unsure, int, float, complex, variable)
__support_component = int | float


def set_fastprint(value: bool) -> None:
    ...


@overload
def set_printarea(*, left: int, right: int, up: int, down: int) -> None:
    ...


@overload
def set_printarea() -> None:
    ...


@overload
def reshape(self: matrix, rows: int, cols: int) -> None:
    ...


@overload
def reshape(self: matrix, shape: tuple[int, int]) -> None:
    ...


class matrix:
    rows: int
    cols: int
    total: int

    @property
    def shape(self) -> tuple[int, int]:
        ...

    @property
    def rank(self) -> int | unsure:
        ...

    def __repr__(self) -> str:
        ...

    def __add__(self, other: matrix) -> matrix:
        ...

    def __radd__(self, other: matrix) -> matrix:
        ...

    def __iadd__(self, other: matrix) -> Self:
        ...

    def __sub__(self, other: matrix) -> matrix:
        ...

    def __rsub__(self, other: matrix) -> matrix:
        ...

    def __isub__(self, other: matrix) -> Self:
        ...

    def __mul__(self, other: __support_num) -> matrix:
        ...

    def __rmul__(self, other: __support_num) -> matrix:
        ...

    def __imul__(self, other: __support_num) -> Self:
        ...

    def __mod__(self, other: __support_num) -> matrix:
        ...

    def __rmod__(self, other: __support_num) -> matrix:
        ...

    def __imod__(self, other: __support_num) -> Self:
        ...

    def __divmod__(self, other: __support_num) -> tuple[matrix, matrix]:
        ...

    def __rdivmod__(self, other: __support_num) -> tuple[matrix, matrix]:
        ...

    def __pos__(self) -> matrix:
        ...

    def __neg__(self) -> matrix:
        ...

    def __floordiv__(self, other: __support_num) -> matrix:
        ...

    def __rfloordiv__(self, other: __support_num) -> matrix:
        ...

    def __truediv__(self, other: __support_num) -> matrix:
        ...

    def __rtruediv__(self, other: __support_num) -> matrix:
        ...

    def __ifloordiv__(self, other: __support_num) -> Self:
        ...

    def __itruediv__(self, other: __support_num) -> Self:
        ...

    def __matmul__(self, other: matrix) -> matrix:
        ...

    def __rmatmul__(self, other: matrix) -> matrix:
        ...

    def __len__(self) -> int:
        ...

    @overload
    def __getitem__(self, index: tuple[int, int]) -> variable:
        ...

    @overload
    def __getitem__(self, index: tuple[int | slice, int | slice]) -> matrix:
        ...

    @overload
    def __getitem__(self, index: ellipsis) -> matrix:
        ...

    @overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: matrix) -> matrix:
        ...

    @overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: __support_num) -> matrix:
        ...

    @overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: list[list[__support_num]] | list[list[__support_num_var]]) -> matrix:
        ...

    @overload
    def __setitem__(self, index: tuple[int | slice, int | slice] | ellipsis, value: list[tuple[int, int, __support_num]]) -> matrix:
        ...

    @overload
    def __setitem__(self, index: tuple[int | slice, int | slice], value: ndarray) -> matrix:
        ...

    def __str__(self) -> str:
        ...

    def __conj__(self) -> matrix:
        ...

    def iconj(self) -> Self:
        ...

    def T(self) -> matrix:
        ...

    def iT(self) -> Self:
        ...

    def H(self) -> matrix:
        ...

    def iH(self) -> Self:
        ...

    def __kronecker__(self, other: matrix) -> matrix:
        ...

    def __hadamard__(self, other: matrix) -> matrix:
        ...

    @staticmethod
    def vstack(*matrixs: matrix) -> matrix:
        ...

    @staticmethod
    def hstack(*matrixs: matrix) -> matrix:
        ...

    @overload
    def __init__(self, rows: int, cols: int, *, fill: __support_num = Unsure) -> None:
        ...

    @overload
    def __init__(self, matrix: list[list[__support_num]], *, fill: __support_num = Unsure) -> None:
        ...

    @overload
    def __init__(self, matrix: list[tuple[int, int, __support_num]], *, fill: __support_num = Unsure) -> None:
        ...

    @overload
    def __init__(self, matrix: ndarray) -> None:
        ...

    @overload
    def __init__(self, matrix: matrix) -> None:
        ...


class vector(matrix):
    @property
    def is_horizontal(self) -> bool:
        ...

    @property
    def is_vertical(self) -> bool:
        ...

    def __conj__(self) -> vector:
        ...

    def iconj(self) -> Self:
        ...

    def T(self) -> vector:
        ...

    def iT(self) -> Self:
        ...

    def H(self) -> vector:
        ...

    def iH(self) -> Self:
        ...

    def __dot__(self, other: matrix) -> variable:
        ...

    def minkowsi_distance(self, p: int) -> errordouble:
        ...

    def L1(self) -> errordouble:
        ...

    def L2(self) -> errordouble:
        ...

    def Linf(self) -> errordouble:
        ...

    @overload
    def __init__(self, dims: int, *, fill: __support_num = Unsure, is_horizontal: bool | None = None) -> None:
        ...

    @overload
    def __init__(self, vector: list[__support_num], *, is_horizontal: bool | None = None) -> None:
        ...

    @overload
    def __init__(self, vector: list[tuple[int, __support_num]], *, fill: __support_num = Unsure, is_horizontal: bool | None = None) -> None:
        ...

    @overload
    def __init__(self, vector: ndarray, *, is_horizontal: bool | None = None) -> None:
        ...

    @overload
    def __init__(self, vector: vector, *, is_horizontal: bool | None = None) -> None:
        ...

    @overload
    def __init__(self, vector: matrix, *, is_horizontal: bool | None = None) -> None:
        ...
