#include "program_providers.h"

#include "widget_config_program.h"

template<Cyclogram::Type t> struct CyclogramTypeConv { static const char *toString() { return "Unknown"; } };
template<> struct CyclogramTypeConv<Cyclogram::Simple> { static const char *toString() { return "Simple"; } };
template<> struct CyclogramTypeConv<Cyclogram::Impulse> { static const char *toString() { return "Impulse"; } };

template<TimeConstraints c> struct TimeConstraintsConv { static const char *toString() { return "Unknown"; } };
template<> struct TimeConstraintsConv<AllTime> { static const char *toString() { return "AllTime"; } };
template<> struct TimeConstraintsConv<StrictEquality> { static const char *toString() { return "StrictEquality"; } };
template<> struct TimeConstraintsConv<EveryDay> { static const char *toString() { return "EveryDay"; } };
template<> struct TimeConstraintsConv<EveryMonth> { static const char *toString() { return "EveryMonth"; } };
template<> struct TimeConstraintsConv<EveryYear> { static const char *toString() { return "EveryYear"; } };

static const int arr_size = 48;

static const char *nameAttr = "name";
static const char *inputAttr = "input";
static const char *outputAttr = "output";
static const char *constrainsAttr = "constrains";
static const char *fromAttr = "from";
static const char *toAttr = "to";
static const char *dateTimeFormat = "dd.MM.yy hh:mm:ss";
static const char *cyclogramTypeAttr = "cyclogramType";
static const char *cyclogramCountAttr = "cyclogramCount";
static const char *cyclogramImpulseDurationAttr = "cyclogramImpulseDuration";
static const char *cyclogramPauseDurationAttr = "cyclogramPauseDuration";
static const char *lowBoundAttr = "lowBound";
static const char *highBoundAttr = "highBound";
static const char *proportionalAttr = "proportional";
static const char *integralAttr = "integral";
static const char *differentialAttr = "differential";
static const char *inverseAttr = "inverse";
static const char *desiredAttr = "desired";

static void dateTimeToArray(const QDateTime &dt, QByteArray &slot, int offset)
{
    slot[offset] = dt.time().second();
    slot[offset + 1] = dt.time().minute();
    slot[offset + 2] = dt.time().hour();
    slot[offset + 3] = dt.date().day();
    slot[offset + 4] = dt.date().month();
    slot[offset + 5] = dt.date().year() % 100;
}

static void cyclogramToArray(const Cyclogram &c, QByteArray &slot, int offset)
{
    slot[offset] = c.type == Cyclogram::Simple? 0: 1;

    switch (c.type)
    {
    case Cyclogram::Simple:
        break;
    case Cyclogram::Impulse:
        slot[offset + 1] = c.count & 0xFF;
        slot[offset + 2] = (c.count >> 8) & 0xFF;
        slot[offset + 3] = c.impulseDuration & 0xFF;
        slot[offset + 4] = (c.impulseDuration >> 8) & 0xFF;
        slot[offset + 5] = c.pauseDuration & 0xFF;
        slot[offset + 6] = (c.pauseDuration >> 8) & 0xFF;
        break;
    }
}

static QDateTime arrayToDateTime(const QByteArray &slot, int offset)
{
    QDate d(slot.at(offset + 5) + 2000, slot.at(offset + 4), slot.at(offset + 3));
    QTime t(slot.at(offset + 2), slot.at(offset + 1), slot.at(offset));

    return QDateTime(d, t);
}

static Cyclogram arrayToCyclogram(const QByteArray &slot, int offset)
{
    Cyclogram c;
    c.type = static_cast<Cyclogram::Type>(slot.at(offset));
    if (c.type != Cyclogram::Simple && c.type != Cyclogram::Impulse)
        c.type = Cyclogram::Simple;

    switch (c.type)
    {
    case Cyclogram::Simple:
        break;
    case Cyclogram::Impulse:
        c.count = (quint8)slot.at(offset + 1) + ((quint8)slot.at(offset + 2) << 8);
        c.impulseDuration = (quint8)slot.at(offset + 3) + ((quint8)slot.at(offset + 4) << 8);
        c.pauseDuration = (quint8)slot.at(offset + 5) + ((quint8)slot.at(offset + 6) << 8);
        break;
    }

    return c;
}

static void constrainsToElement(int c, QDomElement &elem)
{
    switch (c)
    {
    case AllTime:
        elem.setAttribute(constrainsAttr, TimeConstraintsConv<AllTime>::toString());
        break;
    case StrictEquality:
        elem.setAttribute(constrainsAttr, TimeConstraintsConv<StrictEquality>::toString());
        break;
    case EveryDay:
        elem.setAttribute(constrainsAttr, TimeConstraintsConv<EveryDay>::toString());
        break;
    case EveryMonth:
        elem.setAttribute(constrainsAttr, TimeConstraintsConv<EveryMonth>::toString());
        break;
    case EveryYear:
        elem.setAttribute(constrainsAttr, TimeConstraintsConv<EveryYear>::toString());
        break;
    default:
        break;
    }
}

static int constrainsFromElement(QDomElement &elem)
{
    QString c = elem.attribute(constrainsAttr);
    if (c == TimeConstraintsConv<AllTime>::toString())
        return AllTime;
    else if (c == TimeConstraintsConv<StrictEquality>::toString())
        return StrictEquality;
    else if (c == TimeConstraintsConv<EveryDay>::toString())
        return EveryDay;
    else if (c == TimeConstraintsConv<EveryMonth>::toString())
        return EveryMonth;
    else if (c == TimeConstraintsConv<EveryYear>::toString())
        return EveryYear;
    else
        return AllTime;
}

static void cyclogramToElement(const Cyclogram &c, QDomElement &elem)
{
    switch (c.type)
    {
    case Cyclogram::Simple:
        elem.setAttribute(cyclogramTypeAttr, CyclogramTypeConv<Cyclogram::Simple>::toString());
        break;
    case Cyclogram::Impulse:
        elem.setAttribute(cyclogramTypeAttr, CyclogramTypeConv<Cyclogram::Impulse>::toString());
        elem.setAttribute(cyclogramCountAttr, QString::number(c.count));
        elem.setAttribute(cyclogramImpulseDurationAttr, QString::number(c.impulseDuration));
        elem.setAttribute(cyclogramPauseDurationAttr, QString::number(c.pauseDuration));
        break;
    }
}

static void cyclogramFromElement(QDomElement &elem, Cyclogram &c)
{
    QString type = elem.attribute(cyclogramTypeAttr);
    if (type == CyclogramTypeConv<Cyclogram::Simple>::toString()) {
        c.type = Cyclogram::Simple;
    } else if (type == CyclogramTypeConv<Cyclogram::Impulse>::toString()) {
        c.type = Cyclogram::Impulse;
        c.count = elem.attribute(cyclogramCountAttr).toInt();
        c.impulseDuration = elem.attribute(cyclogramImpulseDurationAttr).toInt();
        c.pauseDuration = elem.attribute(cyclogramPauseDurationAttr).toInt();
    }
}

/* EmptyProgram */

EmptyProgramEditorProvider::EmptyProgramEditorProvider(QSharedPointer<EmptyProgram> program) :
    d_program(program) {}

EmptyProgramEditorProvider::~EmptyProgramEditorProvider() {}

QWidget *EmptyProgramEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigEmptyProgram(d_program, hponic, parent));
}

EmptyProgramBinProvider::EmptyProgramBinProvider(QSharedPointer<EmptyProgram> program) :
    d_program(program) {}

EmptyProgramBinProvider::~EmptyProgramBinProvider() {}

QByteArray EmptyProgramBinProvider::toArray()
{
    QByteArray arr(arr_size, char(0));
    arr[0] = d_program->type();
    arr[1] = d_program->id();
    return arr;
}

void EmptyProgramBinProvider::fromArray(const QByteArray &arr)
{
    Q_UNUSED(arr);
}

EmptyProgramXmlProvider::EmptyProgramXmlProvider(QSharedPointer<EmptyProgram> program) :
    d_program(program) {}

EmptyProgramXmlProvider::~EmptyProgramXmlProvider() {}

void EmptyProgramXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_program->name());
}

void EmptyProgramXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    d_program->setName(name);
}

/* TimerControlProgram */

TimerControlProgramEditorProvider::TimerControlProgramEditorProvider(QSharedPointer<TimerControlProgram> program) :
    d_program(program) {}

TimerControlProgramEditorProvider::~TimerControlProgramEditorProvider() {}

QWidget *TimerControlProgramEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigTimerControlProgram(d_program, hponic, parent));
}

TimerControlProgramBinProvider::TimerControlProgramBinProvider(QSharedPointer<TimerControlProgram> program) :
    d_program(program) {}

TimerControlProgramBinProvider::~TimerControlProgramBinProvider() {}

QByteArray TimerControlProgramBinProvider::toArray()
{
    QByteArray arr(arr_size, char(0));
    arr[0] = d_program->type();
    arr[1] = d_program->id();
    arr[2] = d_program->constrains();                   // 1
    dateTimeToArray(d_program->from(), arr, 3);         // 6
    dateTimeToArray(d_program->to(), arr, 9);           // 6
    cyclogramToArray(d_program->cyclogram(), arr, 15);  // 7
    arr[22] = d_program->output();
    return arr;
}

void TimerControlProgramBinProvider::fromArray(const QByteArray &arr)
{
    d_program->setConstrains(arr[2]);           // 1
    QDateTime dt = arrayToDateTime(arr, 3);     // 6
    d_program->setFrom(dt);
    dt = arrayToDateTime(arr, 9);               // 6
    d_program->setTo(dt);

    Cyclogram c = arrayToCyclogram(arr, 15);    // 7
    d_program->setCyclogram(c);

    d_program->setOutput(arr[22]);              // 1
}

TimerControlProgramXmlProvider::TimerControlProgramXmlProvider(QSharedPointer<TimerControlProgram> program) :
    d_program(program) {}

TimerControlProgramXmlProvider::~TimerControlProgramXmlProvider() {}

void TimerControlProgramXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_program->name());
    constrainsToElement(d_program->constrains(), elem);
    elem.setAttribute(fromAttr, d_program->from().toString(dateTimeFormat));
    elem.setAttribute(toAttr, d_program->to().toString(dateTimeFormat));

    cyclogramToElement(d_program->cyclogram(), elem);

    elem.setAttribute(outputAttr, QString::number(d_program->output()));
}

void TimerControlProgramXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int constrains = constrainsFromElement(elem);
    QString from = elem.attribute(fromAttr);
    QString to = elem.attribute(toAttr);
    Cyclogram c;
    cyclogramFromElement(elem, c);
    int output = elem.attribute(outputAttr).toInt();

    d_program->setName(name);

    d_program->setConstrains(constrains);
    d_program->setFrom(QDateTime::fromString(from, dateTimeFormat));
    d_program->setTo(QDateTime::fromString(to, dateTimeFormat));

    d_program->setCyclogram(c);

    d_program->setOutput(output);
}

/* RelayControlProgram */

RelayControlProgramEditorProvider::RelayControlProgramEditorProvider(QSharedPointer<RelayControlProgram> program) :
    d_program(program) {}

RelayControlProgramEditorProvider::~RelayControlProgramEditorProvider() {}

QWidget *RelayControlProgramEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigRelayControlProgram(d_program, hponic, parent));
}

RelayControlProgramBinProvider::RelayControlProgramBinProvider(QSharedPointer<RelayControlProgram> program) :
    d_program(program) {}

RelayControlProgramBinProvider::~RelayControlProgramBinProvider() {}

QByteArray RelayControlProgramBinProvider::toArray()
{
    QByteArray arr(arr_size, char(0));
    arr[0] = d_program->type();
    arr[1] = d_program->id();
    arr[2] = d_program->input();                        // 1
    arr[3] = d_program->constrains();                   // 1
    dateTimeToArray(d_program->from(), arr, 4);         // 6
    dateTimeToArray(d_program->to(), arr, 10);          // 6
    floatToArray(d_program->lowBound(), arr, 16);       // 4
    floatToArray(d_program->highBound(), arr, 20);      // 4
    cyclogramToArray(d_program->cyclogram(), arr, 24);  // 7
    arr[31] = d_program->inverse()? 0x01: 0x00;         // 1
    arr[32] = d_program->output();                      // 1
    return arr;
}

void RelayControlProgramBinProvider::fromArray(const QByteArray &arr)
{
    d_program->setInput(arr[2]);                // 1
    d_program->setConstrains(arr[3]);           // 1
    QDateTime dt = arrayToDateTime(arr, 4);     // 6
    d_program->setFrom(dt);
    dt = arrayToDateTime(arr, 10);              // 6
    d_program->setTo(dt);

    float f = arrayToFloat(arr, 16);            // 4
    d_program->setLowBound(f);
    f = arrayToFloat(arr, 20);                  // 4
    d_program->setHighBound(f);

    Cyclogram c = arrayToCyclogram(arr, 24);    // 7
    d_program->setCyclogram(c);

    d_program->setInverse(arr[31] == 0x01);     // 1
    d_program->setOutput(arr[32]);              // 1
}

RelayControlProgramXmlProvider::RelayControlProgramXmlProvider(QSharedPointer<RelayControlProgram> program) :
    d_program(program) {}

RelayControlProgramXmlProvider::~RelayControlProgramXmlProvider() {}

void RelayControlProgramXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_program->name());
    constrainsToElement(d_program->constrains(), elem);
    elem.setAttribute(fromAttr, d_program->from().toString(dateTimeFormat));
    elem.setAttribute(toAttr, d_program->to().toString(dateTimeFormat));

    elem.setAttribute(inputAttr, QString::number(d_program->input()));
    elem.setAttribute(lowBoundAttr, QString::number(d_program->lowBound()));
    elem.setAttribute(highBoundAttr, QString::number(d_program->highBound()));

    cyclogramToElement(d_program->cyclogram(), elem);

    QVariant v(d_program->inverse());
    elem.setAttribute(inverseAttr, v.toString());

    elem.setAttribute(outputAttr, QString::number(d_program->output()));
}

void RelayControlProgramXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int constrains = constrainsFromElement(elem);
    QString from = elem.attribute(fromAttr);
    QString to = elem.attribute(toAttr);
    int input = elem.attribute(inputAttr).toInt();
    float lowBound = elem.attribute(lowBoundAttr).toFloat();
    float highBound = elem.attribute(highBoundAttr).toFloat();
    Cyclogram c;
    cyclogramFromElement(elem, c);
    int output = elem.attribute(outputAttr).toInt();

    d_program->setName(name);

    d_program->setConstrains(constrains);
    d_program->setFrom(QDateTime::fromString(from, dateTimeFormat));
    d_program->setTo(QDateTime::fromString(to, dateTimeFormat));

    d_program->setInput(input);

    d_program->setLowBound(lowBound);
    d_program->setHighBound(highBound);

    d_program->setCyclogram(c);

    bool inverse =
            elem.attribute(inverseAttr) == QString("true")?
                true:
                false;
    d_program->setInverse(inverse);

    d_program->setOutput(output);
}

/* PidControlProgram */

PidControlProgramEditorProvider::PidControlProgramEditorProvider(QSharedPointer<PidControlProgram> program) :
    d_program(program) {}

PidControlProgramEditorProvider::~PidControlProgramEditorProvider() {}

QWidget *PidControlProgramEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigPidControlProgram(d_program, hponic, parent));
}

PidControlProgramBinProvider::PidControlProgramBinProvider(QSharedPointer<PidControlProgram> program) :
    d_program(program) {}

PidControlProgramBinProvider::~PidControlProgramBinProvider() {}

QByteArray PidControlProgramBinProvider::toArray()
{
    QByteArray arr(arr_size, char(0));
    arr[0] = d_program->type();
    arr[1] = d_program->id();
    arr[2] = d_program->input();                        // 1
    arr[3] = d_program->constrains();                   // 1
    dateTimeToArray(d_program->from(), arr, 4);         // 6
    dateTimeToArray(d_program->to(), arr, 10);          // 6
    floatToArray(d_program->desired(), arr, 16);        // 4
    floatToArray(d_program->proportional(), arr, 20);   // 4
    floatToArray(d_program->integral(), arr, 24);       // 4
    floatToArray(d_program->differential(), arr, 28);   // 4
    arr[32] = d_program->inverse()? 0x01: 0x00;         // 1
    arr[33] = d_program->output();                      // 1
    return arr;
}

void PidControlProgramBinProvider::fromArray(const QByteArray &arr)
{
    d_program->setInput(arr[2]);                // 1
    d_program->setConstrains(arr[3]);           // 1
    QDateTime dt = arrayToDateTime(arr, 4);     // 6
    d_program->setFrom(dt);
    dt = arrayToDateTime(arr, 10);              // 6
    d_program->setTo(dt);

    float f = arrayToFloat(arr, 16);            // 4
    d_program->setDesired(f);
    f = arrayToFloat(arr, 20);                  // 4
    d_program->setProportional(f);
    f = arrayToFloat(arr, 24);                  // 4
    d_program->setIntegral(f);
    f = arrayToFloat(arr, 28);                  // 4
    d_program->setDifferential(f);

    d_program->setInverse(arr[32] == 0x01);     // 1
    d_program->setOutput(arr[33]);              // 1
}

PidControlProgramXmlProvider::PidControlProgramXmlProvider(QSharedPointer<PidControlProgram> program) :
    d_program(program) {}

PidControlProgramXmlProvider::~PidControlProgramXmlProvider() {}

void PidControlProgramXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_program->name());
    constrainsToElement(d_program->constrains(), elem);
    elem.setAttribute(fromAttr, d_program->from().toString(dateTimeFormat));
    elem.setAttribute(toAttr, d_program->to().toString(dateTimeFormat));

    elem.setAttribute(inputAttr, QString::number(d_program->input()));

    elem.setAttribute(desiredAttr, QString::number(d_program->desired()));

    elem.setAttribute(proportionalAttr, QString::number(d_program->proportional()));
    elem.setAttribute(integralAttr, QString::number(d_program->integral()));
    elem.setAttribute(differentialAttr, QString::number(d_program->differential()));

    QVariant v(d_program->inverse());
    elem.setAttribute(inverseAttr, v.toString());

    elem.setAttribute(outputAttr, QString::number(d_program->output()));
}

void PidControlProgramXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int constrains = constrainsFromElement(elem);
    QString from = elem.attribute(fromAttr);
    QString to = elem.attribute(toAttr);
    int input = elem.attribute(inputAttr).toInt();
    float desired = elem.attribute(desiredAttr).toFloat();
    float proportional = elem.attribute(proportionalAttr).toFloat();
    float integral = elem.attribute(integralAttr).toFloat();
    float differential = elem.attribute(differentialAttr).toFloat();
    int output = elem.attribute(outputAttr).toInt();

    d_program->setName(name);

    d_program->setConstrains(constrains);
    d_program->setFrom(QDateTime::fromString(from, dateTimeFormat));
    d_program->setTo(QDateTime::fromString(to, dateTimeFormat));

    d_program->setInput(input);

    d_program->setDesired(desired);

    d_program->setProportional(proportional);
    d_program->setIntegral(integral);
    d_program->setDifferential(differential);

    bool inverse =
            elem.attribute(inverseAttr) == QString("true")?
                true:
                false;
    d_program->setInverse(inverse);

    d_program->setOutput(output);
}

/* ButtonControlProgram */

ButtonControlProgramEditorProvider::ButtonControlProgramEditorProvider(QSharedPointer<ButtonControlProgram> program) :
    d_program(program) {}

ButtonControlProgramEditorProvider::~ButtonControlProgramEditorProvider() {}

QWidget *ButtonControlProgramEditorProvider::createEditor(QSharedPointer<Hponic> hponic, QWidget *parent)
{
    return (new WidgetConfigButtonControlProgram(d_program, hponic, parent));
}

ButtonControlProgramBinProvider::ButtonControlProgramBinProvider(QSharedPointer<ButtonControlProgram> program) :
    d_program(program) {}

ButtonControlProgramBinProvider::~ButtonControlProgramBinProvider() {}

QByteArray ButtonControlProgramBinProvider::toArray()
{
    QByteArray arr(arr_size, char(0));
    arr[0] = d_program->type();
    arr[1] = d_program->id();
    arr[2] = d_program->input();                      // 1
    arr[3] = d_program->output();                     // 1
    return arr;
}

void ButtonControlProgramBinProvider::fromArray(const QByteArray &arr)
{
    d_program->setInput(arr[2]);               // 1
    d_program->setOutput(arr[3]);              // 1
}

ButtonControlProgramXmlProvider::ButtonControlProgramXmlProvider(QSharedPointer<ButtonControlProgram> program) :
    d_program(program) {}

ButtonControlProgramXmlProvider::~ButtonControlProgramXmlProvider() {}

void ButtonControlProgramXmlProvider::toElement(QDomElement &elem)
{
    elem.setAttribute(nameAttr, d_program->name());
    elem.setAttribute(inputAttr, QString::number(d_program->input()));
    elem.setAttribute(outputAttr, QString::number(d_program->output()));
}

void ButtonControlProgramXmlProvider::fromElement(QDomElement &elem)
{
    QString name = elem.attribute(nameAttr);
    int input = elem.attribute(inputAttr).toInt();
    int output = elem.attribute(outputAttr).toInt();

    d_program->setName(name);

    d_program->setInput(input);
    d_program->setOutput(output);
}
