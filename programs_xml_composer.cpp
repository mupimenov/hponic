#include "programs_xml_composer.h"

template<ProgramType t> struct ProgramTypeConv { static const char *toString() { return "Unknown"; } };
template<> struct ProgramTypeConv<EmptyProgramType> { static const char *toString() { return "EmptyProgram"; } };
template<> struct ProgramTypeConv<TimerControlType> { static const char *toString() { return "TimerControlProgram"; } };
template<> struct ProgramTypeConv<RelayControlType> { static const char *toString() { return "RelayControlProgram"; } };
template<> struct ProgramTypeConv<PidControlType> { static const char *toString() { return "PidControlProgram"; } };

template<Cyclogram::Type t> struct CyclogramTypeConv { static const char *toString() { return "Unknown"; } };
template<> struct CyclogramTypeConv<Cyclogram::Simple> { static const char *toString() { return "Simple"; } };
template<> struct CyclogramTypeConv<Cyclogram::Impulse> { static const char *toString() { return "Impulse"; } };

ProgramsXmlComposer::ProgramsXmlComposer(QObject *parent) : QObject(parent)
{

}

const char *ProgramsXmlComposerV1::programsTag = "programs";
const char *ProgramsXmlComposerV1::programTag = "program";
const char *ProgramsXmlComposerV1::typeAttr = "type";
const char *ProgramsXmlComposerV1::idAttr = "id";
const char *ProgramsXmlComposerV1::nameAttr = "name";
const char *ProgramsXmlComposerV1::inputAttr = "input";
const char *ProgramsXmlComposerV1::outputAttr = "output";
const char *ProgramsXmlComposerV1::fromAttr = "from";
const char *ProgramsXmlComposerV1::toAttr = "to";
const char *ProgramsXmlComposerV1::dateTimeFormat = "dd.MM.yy hh:mm:ss";
const char *ProgramsXmlComposerV1::cyclogramTypeAttr = "cyclogramType";
const char *ProgramsXmlComposerV1::cyclogramCountAttr = "cyclogramCount";
const char *ProgramsXmlComposerV1::cyclogramImpulseDurationAttr = "cyclogramImpulseDuration";
const char *ProgramsXmlComposerV1::cyclogramPauseDurationAttr = "cyclogramPauseDuration";
const char *ProgramsXmlComposerV1::lowBoundAttr = "lowBound";
const char *ProgramsXmlComposerV1::highBoundAttr = "highBound";
const char *ProgramsXmlComposerV1::proportionalAttr = "proportional";
const char *ProgramsXmlComposerV1::integralAttr = "integral";
const char *ProgramsXmlComposerV1::differentialAttr = "differential";

ProgramsXmlComposerV1::ProgramsXmlComposerV1(QObject *parent) : ProgramsXmlComposer(parent)
{

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
            child.setAttribute(fromAttr, QDateTime().toString(dateTimeFormat));
            child.setAttribute(toAttr, QDateTime().toString(dateTimeFormat));
            child.setAttribute(inputAttr, QString::number(relayControlProgram->input()));
            child.setAttribute(lowBoundAttr, QString::number(relayControlProgram->lowBound()));
            child.setAttribute(highBoundAttr, QString::number(relayControlProgram->highBound()));
            cyclogramToElement(relayControlProgram->cyclogram(), elem);
            child.setAttribute(outputAttr, QString::number(relayControlProgram->output()));
            break;
        }
        case PidControlType:
        {
            QSharedPointer<PidControlProgram> relayControlProgram = ProgramConv::toProgram<PidControlProgram>(program);

            child.setAttribute(typeAttr, ProgramTypeConv<RelayControlType>::toString());
            child.setAttribute(fromAttr, QDateTime().toString(dateTimeFormat));
            child.setAttribute(toAttr, QDateTime().toString(dateTimeFormat));
            child.setAttribute(inputAttr, QString::number(relayControlProgram->input()));
            child.setAttribute(proportionalAttr, QString::number(relayControlProgram->proportional()));
            child.setAttribute(integralAttr, QString::number(relayControlProgram->integral()));
            child.setAttribute(differentialAttr, QString::number(relayControlProgram->differential()));
            child.setAttribute(outputAttr, QString::number(relayControlProgram->output()));
            break;
        }
        default:
            break;
        }

        elem.appendChild(child);
    }

    return elem;
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

                QString from = child.attribute(fromAttr);
                QString to = child.attribute(toAttr);
                Cyclogram c;
                cyclogramFromElement(child, c);
                int output = child.attribute(outputAttr).toInt();

                timerControlProgram->setName(name);
                timerControlProgram->setFrom(QDateTime::fromString(from, dateTimeFormat));
                timerControlProgram->setTo(QDateTime::fromString(to, dateTimeFormat));
                timerControlProgram->setCyclogram(c);
                timerControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(timerControlProgram));

            } else if (type == ProgramTypeConv<RelayControlType>::toString()) {
                RelayControlProgram *relayControlProgram = new RelayControlProgram(id);

                //QString from = child.attribute(fromAttr);
                //QString to = child.attribute(toAttr);
                int input = child.attribute(inputAttr).toInt();
                float lowBound = child.attribute(lowBoundAttr).toFloat();
                float highBound = child.attribute(highBoundAttr).toFloat();
                Cyclogram c;
                cyclogramFromElement(child, c);
                int output = child.attribute(outputAttr).toInt();

                relayControlProgram->setName(name);
                relayControlProgram->setInput(input);
                relayControlProgram->setLowBound(lowBound);
                relayControlProgram->setHighBound(highBound);
                //relayControlProgram->setFrom(QDateTime::fromString(from, dateTimeFormat));
                //relayControlProgram->setTo(QDateTime::fromString(to, dateTimeFormat));
                relayControlProgram->setCyclogram(c);
                relayControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(relayControlProgram));

            } else if (type == ProgramTypeConv<PidControlType>::toString()) {
                PidControlProgram *pidControlProgram = new PidControlProgram(id);

                //QString from = child.attribute(fromAttr);
                //QString to = child.attribute(toAttr);
                int input = child.attribute(inputAttr).toInt();
                float proportional = child.attribute(proportionalAttr).toFloat();
                float integral = child.attribute(integralAttr).toFloat();
                float differential = child.attribute(differentialAttr).toFloat();
                int output = child.attribute(outputAttr).toInt();

                pidControlProgram->setName(name);
                pidControlProgram->setInput(input);
                pidControlProgram->setProportional(proportional);
                pidControlProgram->setIntegral(integral);
                pidControlProgram->setDifferential(differential);
                //pidControlProgram->setFrom(QDateTime::fromString(from, dateTimeFormat));
                //pidControlProgram->setTo(QDateTime::fromString(to, dateTimeFormat));
                pidControlProgram->setOutput(output);

                programs.append(QSharedPointer<Program>(pidControlProgram));

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
