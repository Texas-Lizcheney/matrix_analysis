import typing
import var as var

__support_num = typing.Union[None, int, float, complex]
__support_component = typing.Union[int, float]


class matrix:
    rows: int
    cols: int