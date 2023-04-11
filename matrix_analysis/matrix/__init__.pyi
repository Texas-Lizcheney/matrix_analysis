import typing
from ..var import variable

__support_num = typing.Union[None, int, float, complex, variable]
__support_component = typing.Union[int, float]


def set_fastprint(value: bool) -> None:
    ...


def set_printarea(left: int, right: int, up: int, down: int) -> None:
    ...


class matrix:
    rows: int
    cols: int
    total: int

    def __init__(self, rows: int, cols: int, /, fill: __support_num) -> None:
        ...

    def __repr__(self) -> str:
        ...
