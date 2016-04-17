#include "program.h"

Program::Program(int __id, int __type, QObject *parent) : QObject(parent),
    d_id(__id),
    d_type(__type)
{

}

Program::~Program()
{

}

void Program::setName(const QString &__name)
{
    d_name = __name;
    Q_EMIT changed(this);
}

const QString &Program::name() const
{
    return d_name;
}

int Program::id() const
{
    return d_id;
}

int Program::type() const
{
    return d_type;
}

EmptyProgram::EmptyProgram(int __id, QObject *parent) : Program(__id, EmptyProgramType, parent)
{
    setName(tr("Empty program"));
}

EmptyProgram::~EmptyProgram()
{

}

Cyclogram::Cyclogram() :
    type(Simple),
    count(1),
    impulseDuration(0),
    pauseDuration(0)
{

}

Cyclogram::~Cyclogram()
{

}

TimerControlProgram::TimerControlProgram(int __id, QObject *parent) : Program(__id, TimerControlType, parent),
    d_constrains(ALL_TIME),
    d_output(0)
{
    setName(tr("Timer control program"));
}

TimerControlProgram::~TimerControlProgram()
{

}

void TimerControlProgram::setConstrains(int mode)
{
    d_constrains = mode;
    Q_EMIT changed(this);
}

int TimerControlProgram::constrains() const
{
    return d_constrains;
}

void TimerControlProgram::setOutput(int __id)
{
    d_output = __id;
    Q_EMIT changed(this);
}

int TimerControlProgram::output() const
{
    return d_output;
}

void TimerControlProgram::setFrom(const QDateTime &__from)
{
    d_from = __from;
    Q_EMIT changed(this);
}

const QDateTime &TimerControlProgram::from() const
{
    return d_from;
}

void TimerControlProgram::setTo(const QDateTime &__to)
{
    d_to = __to;
    Q_EMIT changed(this);
}

const QDateTime &TimerControlProgram::to() const
{
    return d_to;
}

void TimerControlProgram::setCyclogram(const Cyclogram &__cyclogram)
{
    d_cyclogram = __cyclogram;
    Q_EMIT changed(this);
}

const Cyclogram &TimerControlProgram::cyclogram() const
{
    return d_cyclogram;
}

RelayControlProgram::RelayControlProgram(int __id, QObject *parent) : Program(__id, RelayControlType, parent),
    d_input(0),
    d_constrains(ALL_TIME),
    d_lowBound(0.0f),
    d_highBound(0.0f),
    d_output(0)
{
    setName(tr("Relay control program"));
}

RelayControlProgram::~RelayControlProgram()
{

}

void RelayControlProgram::setInput(int __id)
{
    d_input = __id;
    Q_EMIT changed(this);
}

int RelayControlProgram::input() const
{
    return d_input;
}

void RelayControlProgram::setConstrains(int constrains)
{
    d_constrains = constrains;
    Q_EMIT changed(this);
}

int RelayControlProgram::constrains() const
{
    return d_constrains;
}

void RelayControlProgram::setFrom(const QDateTime &__from)
{
    d_from = __from;
    Q_EMIT changed(this);
}

const QDateTime &RelayControlProgram::from() const
{
    return d_from;
}

void RelayControlProgram::setTo(const QDateTime &__to)
{
    d_to = __to;
    Q_EMIT changed(this);
}

const QDateTime &RelayControlProgram::to() const
{
    return d_to;
}

void RelayControlProgram::setOutput(int __id)
{
    d_output = __id;
    Q_EMIT changed(this);
}

int RelayControlProgram::output() const
{
    return d_output;
}

void RelayControlProgram::setLowBound(float __lowBound)
{
    d_lowBound = __lowBound;
    Q_EMIT changed(this);
}

float RelayControlProgram::lowBound() const
{
    return d_lowBound;
}

void RelayControlProgram::setHighBound(float __highBound)
{
    d_highBound = __highBound;
    Q_EMIT changed(this);
}

float RelayControlProgram::highBound() const
{
    return d_highBound;
}

void RelayControlProgram::setCyclogram(const Cyclogram &__cyclogram)
{
    d_cyclogram = __cyclogram;
    Q_EMIT changed(this);
}

const Cyclogram &RelayControlProgram::cyclogram() const
{
    return d_cyclogram;
}

PidControlProgram::PidControlProgram(int __id, QObject *parent) : Program(__id, PidControlType, parent),
    d_input(0),
    d_constrains(ALL_TIME),
    d_proportional(1.0f),
    d_integral(0.0f),
    d_differential(0.0f),
    d_output(0)
{
    setName(tr("PID control program"));
}

PidControlProgram::~PidControlProgram()
{

}

void PidControlProgram::setInput(int __id)
{
    d_input = __id;
    Q_EMIT changed(this);
}

int PidControlProgram::input() const
{
    return d_input;
}

void PidControlProgram::setConstrains(int constrains)
{
    d_constrains = constrains;
    Q_EMIT changed(this);
}

int PidControlProgram::constrains() const
{
    return d_constrains;
}

void PidControlProgram::setFrom(const QDateTime &__from)
{
    d_from = __from;
    Q_EMIT changed(this);
}

const QDateTime &PidControlProgram::from() const
{
    return d_from;
}

void PidControlProgram::setTo(const QDateTime &__to)
{
    d_to = __to;
    Q_EMIT changed(this);
}

const QDateTime &PidControlProgram::to() const
{
    return d_to;
}

void PidControlProgram::setOutput(int __id)
{
    d_output = __id;
    Q_EMIT changed(this);
}

int PidControlProgram::output() const
{
    return d_output;
}

void PidControlProgram::setProportional(float __p)
{
    d_proportional = __p;
    Q_EMIT changed(this);
}

float PidControlProgram::proportional() const
{
    return d_proportional;
}

void PidControlProgram::setIntegral(float __i)
{
    d_integral = __i;
    Q_EMIT changed(this);
}

float PidControlProgram::integral() const
{
    return d_integral;
}

void PidControlProgram::setDifferential(float __d)
{
    d_differential = __d;
    Q_EMIT changed(this);
}

float PidControlProgram::differential() const
{
    return d_differential;
}
