from typing import TypeVar, overload, Protocol

T = TypeVar("T", covariant=True)
_E = TypeVar("_E", contravariant=True)


class _SupportConj(Protocol[T]):
    def __conj__(self) -> T:
        ...


def conj(x: _SupportConj[T]) -> T:
    ...


class _SupportExp(Protocol[T]):
    def __exp__(self) -> T:
        ...


def exp(x: _SupportExp[T]) -> T:
    ...


class _SupportLn(Protocol[T]):
    def __ln__(self) -> T:
        ...


def ln(x: _SupportLn[T]) -> T:
    ...


class _SupportSqrt(Protocol[T]):
    def __sqrt__(self) -> T:
        ...


def sqrt(x: _SupportSqrt[T]) -> T:
    ...


class _SupportSin(Protocol[T]):
    def __sin__(self) -> T:
        ...


def sin(x: _SupportSin[T]) -> T:
    ...


class _SupportCos(Protocol[T]):
    def __cos__(self) -> T:
        ...


def cos(x: _SupportCos[T]) -> T:
    ...


class _SupportTan(Protocol[T]):
    def __tan__(self) -> T:
        ...


def tan(x: _SupportTan[T]) -> T:
    ...


class _SupportCot(Protocol[T]):
    def __cot__(self) -> T:
        ...


def cot(x: _SupportCot[T]) -> T:
    ...


class _SupportSec(Protocol[T]):
    def __sec__(self) -> T:
        ...


def sec(x: _SupportSec[T]) -> T:
    ...


class _SupportCsc(Protocol[T]):
    def __csc__(self) -> T:
        ...


def csc(x: _SupportCsc[T]) -> T:
    ...


class _SupportArcsin(Protocol[T]):
    def __arcsin__(self) -> T:
        ...


def arcsin(x: _SupportArcsin[T]) -> T:
    ...


class _SupportArccos(Protocol[T]):
    def __arccos__(self) -> T:
        ...


def arccos(x: _SupportArccos[T]) -> T:
    ...


class _SupportArctan(Protocol[T]):
    def __arctan__(self) -> T:
        ...


def arctan(x: _SupportArctan[T]) -> T:
    ...


class _SupportArccot(Protocol[T]):
    def __arccot__(self) -> T:
        ...


def arccot(x: _SupportArccot[T]) -> T:
    ...


class _SupportArcsec(Protocol[T]):
    def __arcsec__(self) -> T:
        ...


def arcsec(x: _SupportArcsec[T]) -> T:
    ...


class _SupportArccsc(Protocol[T]):
    def __arccsc__(self) -> T:
        ...


def arccsc(x: _SupportArccsc[T]) -> T:
    ...


class _SupportSinh(Protocol[T]):
    def __sinh__(self) -> T:
        ...


def sinh(x: _SupportSinh[T]) -> T:
    ...


class _SupportCosh(Protocol[T]):
    def __cosh__(self) -> T:
        ...


def cosh(x: _SupportCosh[T]) -> T:
    ...


class _SupportTanh(Protocol[T]):
    def __tanh__(self) -> T:
        ...


def tanh(x: _SupportTanh[T]) -> T:
    ...


class _SupportCoth(Protocol[T]):
    def __coth__(self) -> T:
        ...


def coth(x: _SupportCoth[T]) -> T:
    ...


class _SupportSech(Protocol[T]):
    def __sech__(self) -> T:
        ...


def sech(x: _SupportSech[T]) -> T:
    ...


class _SupportCsch(Protocol[T]):
    def __csch__(self) -> T:
        ...


def csch(x: _SupportCsch[T]) -> T:
    ...


class _SupportArcsinh(Protocol[T]):
    def __arcsinh__(self) -> T:
        ...


def arcsinh(x: _SupportArcsinh[T]) -> T:
    ...


class _SupportArccosh(Protocol[T]):
    def __arccosh__(self) -> T:
        ...


def arccosh(x: _SupportArccosh[T]) -> T:
    ...


class _SupportArctanh(Protocol[T]):
    def __arctanh__(self) -> T:
        ...


def arctanh(x: _SupportArctanh[T]) -> T:
    ...


class _SupportArccoth(Protocol[T]):
    def __arccoth__(self) -> T:
        ...


def arccoth(x: _SupportArccoth[T]) -> T:
    ...


class _SupportArcsech(Protocol[T]):
    def __arcsech__(self) -> T:
        ...


def arcsech(x: _SupportArcsech[T]) -> T:
    ...


class _SupportArccsch(Protocol[T]):
    def __arccsch__(self) -> T:
        ...


def arccsch(x: _SupportArccsch[T]) -> T:
    ...


class _SupportLLog(Protocol[_E, T]):
    def __log__(self, _other: _E) -> T:
        ...


class _SupportRLog(Protocol[_E, T]):
    def __rlog__(self, _other: _E) -> T:
        ...


@overload
def log(x: _SupportLLog[_E, T], base: _E) -> T:
    ...


@overload
def log(x: _E, base: _SupportRLog[_E, T]) -> T:
    ...


class _SupportKronecker(Protocol[_E, T]):
    def __kronecker__(self, _other: _E) -> T:
        ...


def kronecker(x: _SupportKronecker[_E, T], y: _SupportKronecker[_E, T]) -> T:
    ...


class _SupportHadamard(Protocol[_E, T]):
    def __kronecker__(self, _other: _E) -> T:
        ...


def hadamard(x: _SupportHadamard[_E, T], y: _SupportHadamard[_E, T]) -> T:
    ...
