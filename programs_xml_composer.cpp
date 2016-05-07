#include "programs_xml_composer.h"

#include <QVariant>

template<ProgramType t> struct ProgramTypeConv { static const char *toString() { return "Unknown"; } };
template<> struct ProgramTypeConv<EmptyProgramType> { static const char *toString() { return "EmptyProgram"; } };
template<> struct ProgramTypeConv<TimerControlType> { static const char *toString() { return "TimerControlProgram"; } };
template<> struct ProgramTypeConv<RelayControlType> { static const char *toString() { return "RelayControlProgram"; } };
template<> struct ProgramTypeConv<PidControlType> { static const char *toString() { return "PidControlProgram"; } };
template<> struct ProgramTypeConv<ButtonControlType> { static const char *toString() { return "ButtonControlProgram"; } };

template<Cyclogram::Type t> struct CyclogramTypeConv { static const char *toString() { return "Unknown"; } };
template<> struct CyclogramTypeConv<Cyclogram::Simple> { static const char *toString() { return "Simple"; } };
template<> struct CyclogramTypeConv<Cyclogram::Impulse> { static const char *toString() { return "Impulse"; } };

ProgramsXmlComposer::ProgramsXmlComposer(QObject *parent) : QObject(parent)
{

}

static const char *programsTag = "programs";
static const char *programTag = "program";
static const char *typeAttr = "type";
static const char *idAttr = "id";
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

ProgramsXmlComposerV1::ProgramsXmlComposerV1(QObject *parent) : ProgramsXmlComposer(parent)
{

}

template<TimeConstraints c> struct TimeConstraintsConv { static const char *toString() { return "Unknown"; } };
template<> struct TimeConstraintsConv<AllTime> { static const char *toString() { return "AllTime"; } };
template<> struct TimeConstraintsConv<StrictEquality> { static const char *toString() { return "StrictEquality"; } };
template<> struct TimeConstraintsConv<EveryDay> { static const char *toString() { return "EveryDay"; } };
template<> struct TimeConstraintsConv<EveryMonth> { static const char *toString() { return "EveryMonth"; } };
template<> struct TimeConstraintsConv<EveryYear> { static const char *toString() { return "EveryYear"; } };

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

QDomElement ProgramsXmlComposerV1::toElement(const QList<QSharedPointer<Program> > &programs, QDomDocument &doc)
{
    QDomElement elem = doc.createElement(programsTag);

    for (int i = 0; i < programs.size(); ++i) {
        QSharedPointer<Program> program = programs[i];
        if (!program)
            continue;

        QDomElement child = doc.createElement(programTag);
        child.setAttribute(idAttr, QString::number(program->id()));
        child.setAttribute(nameAttr, program->name());

        switch (program->type()) {
        case EmptyProgramType:
        {
            child.setAttribute(typeAttr, ProgramTypeConv<EmptyProgramType>::toString());
            break;
        }
        case TimerControlType:
        {
            QSharedPointer<TimerControlProgram> timerControlProgram = ProgramConv::toProgram<TimerControlProgram>(program);

            child.setAttribute(typeAttr, ProgramTypeConv<TimerControlType>::toString());

            constrainsToElement(timerControlProgram->constrains(), child);
            child.setAttribute(fromAttr, timerControlProgram->from().toString(dateTimeFormat));            
            child.setAttribute(toAttr, timerControlProgram->to().toString(dateTimeFormat));

            cyclogramToElement(timerControlProgram->cyclogram(), child);

            child.setAttribute(outputAttr, QString::number(timerControlProgram->output()));
            break;
        }
        case RelayControlType:
        {
            QSharedPointer<RelayControlProgram> relayControlProgram = ProgramConv::toProgram<RelayControlProgram>(program);

            child.setAttribute(typeAttr, ProgramTypeConv<RelayControlType>::toString());

            constrainsToElement(relayControlProgram->constrains(), child);
            child.setAttribute(fromAttr, relayControlProgram->from().toString(dateTimeFormat));
            child.setAttribute(toAttr, relayControlProgram->to().toString(dateTimeFormat));

            child.setAttribute(inputAttr, QString::number(relayControlProgram->input()));
            child.setAttribute(lowBoundAttr, QString::number(relayControlProgram->lowBound()));
            child.setAttribute(highBoundAttr, QString::number(relayControlProgram->highBound()));

            cyclogramToElement(relayControlProgram->cyclogram(), child);

            QVariant v(relayControlProgram->inverse());
            child.setAttribute(inverseAttr, v.toString());

            child.setAttribute(outputAttr, QString::number(relayControlProgram->output()));
            break;
        }
        case PidControlType:
        {
            QSharedPointer<PidControlProgram> pidControlProgram = ProgramConv::toProgram<PidControlProgram>(program);

            child.setAttribute(typeAttr, ProgramTypeConv<PidControlType>::toString());

            constrainsToElement(pidControlProgram->constrains(), child);
            child.setAttribute(fromAttr, pidControlProgram->from().toString(dateTimeFormat));
            child.setAttribute(toAttr, pidControlProgram->to().toString(dateTimeFormat));

            child.setAttribute(inputAttr, QString::number(pidControlProgram->input()));

            child.setAttribute(desiredAttr, QString::number(pidControlProgram->desired()));

            child.setAttribute(proportionalAttr, QString::number(pidControlProgram->proportional()));
            child.setAttribute(integralAttr, QString::number(pidControlProgram->integral()));
            child.setAttribute(differentialAttr, QString::number(pidControlProgram->differential()));

            QVariant v(pidControlProgram->inverse());
            child.setAttribute(inverseAttr, v.toString());

            child.setAttribute(outputAttr, QString::number(pidControlProgram->output()));
            break;
        }
        case ButtonControlType:
        {
            QSharedPointer<ButtonControlProgram> buttonControlProgram = ProgramConv::toProgram<ButtonControlProgram>(program);

            child.setAttribute(typeAttr, ProgramTypeConv<ButtonControlType>::toString());

            child.setAttribute(inputAttr, QString::number(buttonControlProgram->input()));
            child.setAttribute(outputAttr, QString::number(buttonControlProgram->output()));
            break;
        }
        default:
            break;
        }

        elem.appendChild(child);
    }

    return elem;
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

QList<QSharedPointer<Program> > ProgramsXmlComposerV1::fromElement(QDomElement &root)
{
    QList<QSharedPointer<Program> > programs;

    QDomElement elem;
    QDomElement child = root.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == programsTag) {
            elem = child;
            break;
        }
        child = child.nextSiblingElement();
    }

    if (elem.isNull()) return programs;

    child = elem.firstChildElement();
    while (!child.isNull()) {
        if (child.tagName() == programTag) {
            int id = child.attribute(idAttr).toInt();
            QString name = child.attribute(nameAttr);
            QString type = child.attribute(typeAttr);
            if (type.isEmpty())
                continue;

            if (type == ProgramTypeConv<EmptyProgramType>::toString()) {
                EmptyProgram *emptyProgram = new EmptyProgram(id);
                emptyProgram->setName(name);

                programs.append(QSharedPointer<Program>(emptyProgram));

            } else if (type == ProgramTypeConv<TimerControlType>::toString()) {
                TimerControlProgram *timerControlProgram = new TimerControlProgram(id);

                int constrains = constrainsFromElement(child);
                QString from = child.attribute(fromAttr);
                QString to = child.attribute(toAttr);
                Cyclogram c;
                cyclogramFromElement(child, c);                
                int output = child.attribute(outputAttr).toInt();

                timerControlProgram->setName(name);

                timerControlProgram->setConstrains(constrains);
                timerControlProgram->setFrom(QDateTime::fromString(from, dateTimeFormat));
                timerControlProgram->setTo(QDateTime::fromString(to, dateTimeFormat));

                timerControlProgram->setCyclogram(c);

                timerControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(timerControlProgram));

            } else if (type == ProgramTypeConv<RelayControlType>::toString()) {
                RelayControlProgram *relayControlProgram = new RelayControlProgram(id);

                int constrains = constrainsFromElement(child);
                QString from = child.attribute(fromAttr);
                QString to = child.attribute(toAttr);
                int input = child.attribute(inputAttr).toInt();
                float lowBound = child.attribute(lowBoundAttr).toFloat();
                float highBound = child.attribute(highBoundAttr).toFloat();
                Cyclogram c;
                cyclogramFromElement(child, c);
                int output = child.attribute(outputAttr).toInt();

                relayControlProgram->setName(name);

                relayControlProgram->setConstrains(constrains);
                relayControlProgram->setFrom(QDateTime::fromString(from, dateTimeFormat));
                relayControlProgram->setTo(QDateTime::fromString(to, dateTimeFormat));

                relayControlProgram->setInput(input);

                relayControlProgram->setLowBound(lowBound);
                relayControlProgram->setHighBound(highBound);

                relayControlProgram->setCyclogram(c);

                bool inverse =
                        child.attribute(inverseAttr) == QString("true")?
                            true:
                            false;
                relayControlProgram->setInverse(inverse);

                relayControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(relayControlProgram));

            } else if (type == ProgramTypeConv<PidControlType>::toString()) {
                PidControlProgram *pidControlProgram = new PidControlProgram(id);

                int constrains = constrainsFromElement(child);
                QString from = child.attribute(fromAttr);
                QString to = child.attribute(toAttr);
                int input = child.attribute(inputAttr).toInt();
                float desired = child.attribute(desiredAttr).toFloat();
                float proportional = child.attribute(proportionalAttr).toFloat();
                float integral = child.attribute(integralAttr).toFloat();
                float differential = child.attribute(differentialAttr).toFloat();
                int output = child.attribute(outputAttr).toInt();

                pidControlProgram->setName(name);

                pidControlProgram->setConstrains(constrains);
                pidControlProgram->setFrom(QDateTime::fromString(from, dateTimeFormat));
                pidControlProgram->setTo(QDateTime::fromString(to, dateTimeFormat));

                pidControlProgram->setInput(input);

                pidControlProgram->setDesired(desired);

                pidControlProgram->setProportional(proportional);
                pidControlProgram->setIntegral(integral);
                pidControlProgram->setDifferential(differential);

                bool inverse =
                        child.attribute(inverseAttr) == QString("true")?
                            true:
                            false;
                pidControlProgram->setInverse(inverse);

                pidControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(pidControlProgram));

            } else if (type == ProgramTypeConv<ButtonControlType>::toString()) {
                ButtonControlProgram *buttonControlProgram = new ButtonControlProgram(id);

                int input = child.attribute(inputAttr).toInt();
                int output = child.attribute(outputAttr).toInt();

                buttonControlProgram->setName(name);

                buttonControlProgram->setInput(input);
                buttonControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(buttonControlProgram));

            } else {
                EmptyProgram *emptyProgram = new EmptyProgram(id);
                programs.append(QSharedPointer<Program>(emptyProgram));
            }
        }

        child = child.nextSiblingElement();
    }

    return programs;
}

void ProgramsXmlComposerV1::cyclogramToElement(const Cyclogram &c, QDomElement &elem)
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

void ProgramsXmlComposerV1::cyclogramFromElement(QDomElement &elem, Cyclogram &c)
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
