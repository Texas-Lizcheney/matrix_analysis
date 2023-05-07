from typing import overload, TypeVar, TypeAlias
from numpy import ndarray
from ..var import variable
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


class matrix:
    rows: int
    cols: int
    total: int
    shape: tuple[int, int]

    @property
    def rank(self) -> int | unsure:
        ...

    def __repr__(self) -> str:
        ...

    def __add__(self, other: matrix) -> matrix:
        ...

    def __radd__(self, other: matrix) -> matrix:
        ...

    def __iadd__(self, other: matrix) -> matrix:
        ...

    def __sub__(self, other: matrix) -> matrix:
        ...

    def __rsub__(self, other: matrix) -> matrix:
        ...

    def __isub__(self, other: matrix) -> matrix:
        ...

    def __mul__(self, other: __support_num) -> matrix:
        ...

    def __rmul__(self, other: __support_num) -> matrix:
        ...

    def __imul__(self, other: __support_num) -> matrix:
        ...

    def __mod__(self, other: __support_num) -> matrix:
        ...

    def __rmod__(self, other: __support_num) -> matrix:
        ...

    def __imod__(self, other: __support_num) -> matrix:
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

    def __ifloordiv__(self, other: __support_num) -> matrix:
        ...

    def __itruediv__(self, other: __support_num) -> matrix:
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

    def T(self) -> matrix:
        ...

    def H(self) -> matrix:
        ...

    def __kronecker__(self, other: matrix) -> matrix:
        ...

    def __hadamard__(self, other: matrix) -> matrix:
        ...

    @overload
    def reshape(self, rows: int, cols: int) -> None:
        ...

    @overload
    def reshape(self, shape: tuple[int, int]) -> None:
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
