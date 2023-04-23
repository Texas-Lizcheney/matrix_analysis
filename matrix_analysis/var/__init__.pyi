import typing
from ..__init__ import Unsure

__support_num = typing.Union[Unsure, int, float, complex, variable]
__support_component = typing.Union[int, float]


def set_print_precision(precision: int) -> None:
    ...


def set_arg_format(format: str) -> None:
    ...


class variable:
    real: float
    imag: float
    rec: typing.Union[typing.Tuple[float, float], Unsure]
    length: float
    arg: float
    pol: typing.Union[typing.Tuple[float, float], Unsure]
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

    def __eq__(self,other:__support_component)->bool|Unsure:
        ...

    def __ne__(self,other:__support_component)->bool|Unsure:
        ...

    def __add__(self, other: __support_num) -> variable:
        ...

    def __radd__(self, other: __support_num) -> variable:
        ...

    def __iadd__(self, other: __support_num) -> typing.Self:
        ...

    def __sub__(self, other: __support_num) -> variable:
        ...

    def __rsub__(self, other: __support_num) -> variable:
        ...

    def __isub__(self, other: __support_num) -> typing.Self:
        ...

    def __mul__(self, other: __support_num) -> variable:
        ...

    def __rmul__(self, other: __support_num) -> variable:
        ...

    def __imul__(self, other: __support_num) -> typing.Self:
        ...

    def __mod__(self, other: __support_num) -> variable:
        ...

    def __rmod__(self, other: __support_num) -> variable:
        ...

    def __imod__(self, other: __support_num) -> typing.Self:
        ...

    def __divmod__(self, other: __support_num) -> typing.Union[variable, variable]:
        ...

    def __rdivmod__(self, other: __support_num) -> typing.Union[variable, variable]:
        ...

    def __pow__(self, other: __support_num, mod: __support_num) -> variable:
        ...

    def __rpow__(self, other: __support_num, mod: __support_num) -> variable:
        ...

    def __ipow__(self, other: __support_num, mod: __support_num) -> typing.Self:
        ...

    def __neg__(self) -> variable:
        ...

    def __pos__(self) -> variable:
        ...

    def __abs__(self) -> float:
        ...

    def __bool__(self) -> bool:
        ...

    def __invert__(self) -> variable:
        ...

    def __floordiv__(self, other: __support_num) -> variable:
        ...

    def __ifloordiv__(self, other: __support_num) -> typing.Self:
        ...

    def __truediv__(self, other: __support_num) -> variable:
        ...

    def __itruediv__(self, other: __support_num) -> typing.Self:
        ...

    def conj(self) -> variable:
        ...

    def exp(self) -> variable:
        ...

    def ln(self) -> variable:
        ...

    def log(self, base: __support_num) -> variable:
        ...

    def log_asbase(self, x: __support_num) -> variable:
        ...

    def sqrt(self) -> variable:
        ...

    def sin(self) -> variable:
        ...

    def cos(self) -> variable:
        ...

    def tan(self) -> variable:
        ...

    def cot(self) -> variable:
        ...

    def sec(self) -> variable:
        ...

    def csc(self) -> variable:
        ...

    def arcsin(self) -> variable:
        ...

    def arccos(self) -> variable:
        ...

    def arctan(self) -> variable:
        ...

    def arccot(self) -> variable:
        ...

    def arcsec(self) -> variable:
        ...

    def arccsc(self) -> variable:
        ...

    def sinh(self) -> variable:
        ...

    def cosh(self) -> variable:
        ...

    def tanh(self) -> variable:
        ...

    def coth(self) -> variable:
        ...

    def sech(self) -> variable:
        ...

    def csch(self) -> variable:
        ...

    def arcsinh(self) -> variable:
        ...

    def arccosh(self) -> variable:
        ...

    def arctanh(self) -> variable:
        ...

    def arccoth(self) -> variable:
        ...

    def arcsech(self) -> variable:
        ...

    def arccsch(self) -> variable:
        ...


