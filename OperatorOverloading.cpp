#include "MyDefenitions.h"

QTextStream &operator >>(QTextStream &in, ActivationFunc& aFunc)
{
    in >> (quint32&)aFunc;
    return in;
}

QTextStream &operator <<(QTextStream &out, ActivationFunc& aFunc)
{
    out << (quint32&)aFunc;
    return out;
}
