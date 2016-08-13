#include "program.h"

Program::Program(int id, int type, QObject *parent) : QObject(parent),
    d_id(id),
    d_type(type)
{

}

Program::~Program()
{

}

void Program::setName(const QString &name)
{
    d_name = name;
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

void Program::setProviders(QSharedPointer<ProgramProviders> providers)
{
    d_providers = providers;
}

QSharedPointer<ProgramProviders> Program::providers() const
{
    return d_providers;
}

EmptyProgram::EmptyProgram(int id, QObject *parent) :
    Program(id, EmptyProgramType, parent)
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

TimerControlProgram::TimerControlProgram(int id, QObject *parent) :
    Program(id, TimerControlType, parent),
    d_constrains(AllTime),
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

void TimerControlProgram::setOutput(int id)
{
    d_output = id;
    Q_EMIT changed(this);
}

int TimerControlProgram::output() const
{
    return d_output;
}

void TimerControlProgram::setFrom(const QDateTime &from)
{
    d_from = from;
    Q_EMIT changed(this);
}

const QDateTime &TimerControlProgram::from() const
{
    return d_from;
}

void TimerControlProgram::setTo(const QDateTime &to)
{
    d_to = to;
    Q_EMIT changed(this);
}

const QDateTime &TimerControlProgram::to() const
{
    return d_to;
}

void TimerControlProgram::setCyclogram(const Cyclogram &cyclogram)
{
    d_cyclogram = cyclogram;
    Q_EMIT changed(this);
}

const Cyclogram &TimerControlProgram::cyclogram() const
{
    return d_cyclogram;
}

RelayControlProgram::RelayControlProgram(int id, QObject *parent) :
    Program(id, RelayControlType, parent),
    d_input(0),
    d_constrains(AllTime),
    d_lowBound(0.0f),
    d_highBound(0.0f),
    d_inverse(false),
    d_output(0)
{
    setName(tr("Relay control program"));
}

RelayControlProgram::~RelayControlProgram()
{

}

void RelayControlProgram::setInput(int id)
{
    d_input = id;
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

void RelayControlProgram::setFrom(const QDateTime &from)
{
    d_from = from;
    Q_EMIT changed(this);
}

const QDateTime &RelayControlProgram::from() const
{
    return d_from;
}

void RelayControlProgram::setTo(const QDateTime &to)
{
    d_to = to;
    Q_EMIT changed(this);
}

const QDateTime &RelayControlProgram::to() const
{
    return d_to;
}

void RelayControlProgram::setOutput(int id)
{
    d_output = id;
    Q_EMIT changed(this);
}

int RelayControlProgram::output() const
{
    return d_output;
}

void RelayControlProgram::setLowBound(float lowBound)
{
    d_lowBound = lowBound;
    Q_EMIT changed(this);
}

float RelayControlProgram::lowBound() const
{
    return d_lowBound;
}

void RelayControlProgram::setHighBound(float highBound)
{
    d_highBound = highBound;
    Q_EMIT changed(this);
}

float RelayControlProgram::highBound() const
{
    return d_highBound;
}

void RelayControlProgram::setCyclogram(const Cyclogram &cyclogram)
{
    d_cyclogram = cyclogram;
    Q_EMIT changed(this);
}

const Cyclogram &RelayControlProgram::cyclogram() const
{
    return d_cyclogram;
}

void RelayControlProgram::setInverse(bool inverse)
{
    d_inverse = inverse;
    Q_EMIT changed(this);
}

bool RelayControlProgram::inverse() const
{
    return d_inverse;
}

PidControlProgram::PidControlProgram(int id, QObject *parent) :
    Program(id, PidControlType, parent),
    d_input(0),
    d_constrains(AllTime),
    d_desired(1.0f),
    d_proportional(1.0f),
    d_integral(0.0f),
    d_differential(0.0f),
    d_inverse(false),
    d_output(0)
{
    setName(tr("PID control program"));
}

PidControlProgram::~PidControlProgram()
{

}

void PidControlProgram::setInput(int id)
{
    d_input = id;
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

void PidControlProgram::setFrom(const QDateTime &from)
{
    d_from = from;
    Q_EMIT changed(this);
}

const QDateTime &PidControlProgram::from() const
{
    return d_from;
}

void PidControlProgram::setTo(const QDateTime &to)
{
    d_to = to;
    Q_EMIT changed(this);
}

const QDateTime &PidControlProgram::to() const
{
    return d_to;
}

void PidControlProgram::setDesired(float desired)
{
    d_desired = desired;
    Q_EMIT changed(this);
}

float PidControlProgram::desired() const
{
    return d_desired;
}

void PidControlProgram::setOutput(int id)
{
    d_output = id;
    Q_EMIT changed(this);
}

int PidControlProgram::output() const
{
    return d_output;
}

void PidControlProgram::setProportional(float p)
{
    d_proportional = p;
    Q_EMIT changed(this);
}

float PidControlProgram::proportional() const
{
    return d_proportional;
}

void PidControlProgram::setIntegral(float i)
{
    d_integral = i;
    Q_EMIT changed(this);
}

float PidControlProgram::integral() const
{
    return d_integral;
}

void PidControlProgram::setDifferential(float d)
{
    d_differential = d;
    Q_EMIT changed(this);
}

float PidControlProgram::differential() const
{
    return d_differential;
}

void PidControlProgram::setInverse(bool inverse)
{
    d_inverse = inverse;
    Q_EMIT changed(this);
}

bool PidControlProgram::inverse() const
{
    return d_inverse;
}

ButtonControlProgram::ButtonControlProgram(int id, QObject *parent) :
    Program(id, ButtonControlType, parent),
    d_input(0),
    d_output(0)
{
    setName(tr("Button control program"));
}

ButtonControlProgram::~ButtonControlProgram()
{

}

void ButtonControlProgram::setInput(int id)
{
    d_input = id;
    Q_EMIT changed(this);
}

int ButtonControlProgram::input() const
{
    return d_input;
}

void ButtonControlProgram::setOutput(int id)
{
    d_output = id;
    Q_EMIT changed(this);
}

int ButtonControlProgram::output() const
{
    return d_output;
}

ProgramProvidersV1::ProgramProvidersV1(QSharedPointer<ProgramEditorProvider> editorProvider_,
                                       QSharedPointer<ProgramBinProvider> binProvider_,
                                       QSharedPointer<ProgramXmlProvider> xmlProvider_) :
    d_editorProvider(editorProvider_),
    d_binProvider(binProvider_),
    d_xmlProvider(xmlProvider_) {}

QSharedPointer<ProgramEditorProvider> ProgramProvidersV1::editorProvider()
{
    return d_editorProvider;
}

QSharedPointer<ProgramBinProvider> ProgramProvidersV1::binProvider()
{
    return d_binProvider;
}

QSharedPointer<ProgramXmlProvider> ProgramProvidersV1::xmlProvider()
{
    return d_xmlProvider;
}
