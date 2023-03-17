import typing

__support_num = typing.Union[None, int, float, complex, variable]
__support_component = typing.Union[int, float]


class variable:
    real: float
    imag: float
    is_arbitrary: bool

    @typing.overload
    def __init__(self) -> variable: ...

    @typing.overload
    def __init__(self, num: __support_num) -> variable: ...

    @typing.overload
    def __init__(self, real: __support_component,
                 imag: __support_component) -> variable: ...
    
    def __repr__(self) -> str: ...


