import typing
from ..__init__ import Unsure

__support_num = typing.Union[Unsure,
                             errordouble, int, float, complex, variable]
__support_component = typing.Union[errordouble, int, float]


def set_print_precision(precision: int) -> None:
    ...


def set_arg_format(format: typing.Literal["deg", "rad"]) -> None:
    ...


def set_print_error(value: bool) -> None:
    ...


class errordouble:
    value: float
    error: float

    def __repr__(self) -> str:
        ...

    def __add__(self, other: __support_component) -> errordouble:
        ...

    def __radd__(self, other: __support_component) -> errordouble:
        ...

    def __iadd__(self, other: __support_component) -> typing.Self:
        ...

    def __sub__(self, other: __support_component) -> errordouble:
        ...

    def __rsub__(self, other: __support_component) -> errordouble:
        ...

    def __isub__(self, other: __support_component) -> typing.Self:
        ...

    def __mul__(self, other: __support_component) -> errordouble:
        ...

    def __rmul__(self, other: __support_component) -> errordouble:
        ...

    def __imul__(self, other: __support_component) -> typing.Self:
        ...

    def __mod__(self, other: __support_component) -> errordouble:
        ...

    def __rmod__(self, other: __support_component) -> errordouble:
        ...

    def __imod__(self, other: __support_component) -> typing.Self:
        ...

    def __divmod__(self, other: __support_component) -> tuple[errordouble, errordouble]:
        ...

    def __rdivmod__(self, other: __support_component) -> tuple[errordouble, errordouble]:
        ...

    @typing.overload
    def __pow__(self, other: __support_component) -> errordouble:
        ...

    @typing.overload
    def __pow__(self, other: __support_component, mod: __support_num | None) -> errordouble:
        ...

    @typing.overload
    def __rpow__(self, other: __support_component) -> errordouble:
        ...

    @typing.overload
    def __rpow__(self, other: __support_component, mod: __support_num | None) -> errordouble:
        ...

    @typing.overload
    def __ipow__(self, other: __support_component) -> typing.Self:
        ...

    @typing.overload
    def __ipow__(self, other: __support_component, mod: __support_num | None) -> typing.Self:
        ...

    def __neg__(self) -> errordouble:
        ...

    def __pos__(self) -> errordouble:
        ...

    def __abs__(self) -> errordouble:
        ...

    def __bool__(self) -> errordouble:
        ...

    def __int__(self) -> int:
        ...

    def __float__(self) -> float:
        ...

    def __floordiv__(self, other: __support_component) -> errordouble:
        ...

    def __rfloordiv__(self, other: __support_component) -> errordouble:
        ...

    def __ifloordiv__(self, other: __support_component) -> typing.Self:
        ...

    def __truediv__(self, other: __support_component) -> errordouble:
        ...

    def __rtruediv__(self, other: __support_component) -> errordouble:
        ...

    def __itruediv__(self, other: __support_component) -> typing.Self:
        ...

    @typing.overload
    def __init__(self, value: float) -> None:
        ...

    @typing.overload
    def __init__(self, value: float, error: float) -> None:
        ...

    def __round__(self, __ndigits: int, /) ->float:
        ...


class variable:
    real: errordouble
    imag: errordouble
    rec: tuple[errordouble, errordouble] | Unsure
    r: float
    arg: float
    pol: tuple[errordouble, errordouble] | Unsure
    is_arbitrary: bool

    def __repr__(self) -> str:
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

    def __divmod__(self, other: __support_num) -> tuple[variable, variable]:
        ...

    def __rdivmod__(self, other: __support_num) -> tuple[variable, variable]:
        ...

    @typing.overload
    def __pow__(self, other: __support_num) -> variable:
        ...

    @typing.overload
    def __pow__(self, other: __support_num, mod: __support_num | None) -> variable:
        ...

    @typing.overload
    def __rpow__(self, other: __support_num) -> variable:
        ...

    @typing.overload
    def __rpow__(self, other: __support_num, mod: __support_num | None) -> variable:
        ...

    @typing.overload
    def __ipow__(self, other: __support_num) -> typing.Self:
        ...

    @typing.overload
    def __ipow__(self, other: __support_num, mod: __support_num | None) -> typing.Self:
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

    def __rfloordiv__(self, other: __support_num) -> variable:
        ...

    def __ifloordiv__(self, other: __support_num) -> typing.Self:
        ...

    def __truediv__(self, other: __support_num) -> variable:
        ...

    def __rtruediv__(self, other: __support_num) -> variable:
        ...

    def __itruediv__(self, other: __support_num) -> typing.Self:
        ...

    def __str__(self) -> str:
        ...

    def __eq__(self, other: __support_component) -> bool | Unsure:
        ...

    def __ne__(self, other: __support_component) -> bool | Unsure:
        ...

    def __conj__(self) -> variable:
        ...

    def __exp__(self) -> variable:
        ...

    def __ln__(self) -> variable:
        ...

    def __sqrt__(self) -> variable:
        ...

    def __sin__(self) -> variable:
        ...

    def __cos__(self) -> variable:
        ...

    def __tan__(self) -> variable:
        ...

    def __cot__(self) -> variable:
        ...

    def __sec__(self) -> variable:
        ...

    def __csc__(self) -> variable:
        ...

    def __arcsin__(self) -> variable:
        ...

    def __arccos__(self) -> variable:
        ...

    def __arctan__(self) -> variable:
        ...

    def __arccot__(self) -> variable:
        ...

    def __arcsec__(self) -> variable:
        ...

    def __arccsc__(self) -> variable:
        ...

    def __sinh__(self) -> variable:
        ...

    def __cosh__(self) -> variable:
        ...

    def __tanh__(self) -> variable:
        ...

    def __coth__(self) -> variable:
        ...

    def __sech__(self) -> variable:
        ...

    def __csch__(self) -> variable:
        ...

    def __arcsinh__(self) -> variable:
        ...

    def __arccosh__(self) -> variable:
        ...

    def __arctanh__(self) -> variable:
        ...

    def __arccoth__(self) -> variable:
        ...

    def __arcsech__(self) -> variable:
        ...

    def __arccsch__(self) -> variable:
        ...

    def __log__(self, base: __support_num) -> variable:
        ...

    def __rlog__(self, x: __support_num) -> variable:
        ...

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

    @typing.overload
    def __init__(self, r: __support_component,
                 arg: __support_component) -> variable:
        ...
