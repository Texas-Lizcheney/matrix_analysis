import typing
from ..var import variable

__support_num = typing.Union[None, int, float, complex, variable]
__support_component = typing.Union[int, float]


class matrix:
    rows: int
    cols: int
    total_elements: int
    elements: list[list[variable]]

    def __init__(self, rows: int, cols: int, number: __support_num) -> None:
        ...
