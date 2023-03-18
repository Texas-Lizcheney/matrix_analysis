import typing

support_num = typing.Union[None, int, float, complex, variable]
support_component = typing.Union[int, float]


class variable:
    real: float
    imag: float
    is_arbitrary: bool

    @typing.overload()
    def __init__(self) -> variable: ...

    @typing.overload()
    def __init__(self, num: support_num) -> variable: ...

    @typing.overload()
    def __init__(self, real: support_component,
                 imag: support_component) -> variable: ...
    
    def __repr__(self) -> str: ...

    def __str__(self) -> str: ...

