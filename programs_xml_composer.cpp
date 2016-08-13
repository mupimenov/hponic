#include "programs_xml_composer.h"

#include <QVariant>

#include "program_manager.h"
#include "program_providers.h"

template<ProgramType t> struct ProgramTypeConv { static const char *toString() { return "Unknown"; } };
template<> struct ProgramTypeConv<EmptyProgramType> { static const char *toString() { return "EmptyProgram"; } };
template<> struct ProgramTypeConv<TimerControlType> { static const char *toString() { return "TimerControlProgram"; } };
template<> struct ProgramTypeConv<RelayControlType> { static const char *toString() { return "RelayControlProgram"; } };
template<> struct ProgramTypeConv<PidControlType> { static const char *toString() { return "PidControlProgram"; } };
template<> struct ProgramTypeConv<ButtonControlType> { static const char *toString() { return "ButtonControlProgram"; } };

static const char *programsTag = "programs";
static const char *programTag = "program";
static const char *typeAttr = "type";
static const char *idAttr = "id";

ProgramsXmlComposer::ProgramsXmlComposer(QObject *parent) :
    QObject(parent) {}

ProgramsXmlComposerV1::ProgramsXmlComposerV1(QObject *parent) :
    ProgramsXmlComposer(parent) {}

QDomElement ProgramsXmlComposerV1::toElement(const QList<QSharedPointer<Program> > &programs, QDomDocument &doc)
{
    QDomElement elem = doc.createElement(programsTag);

    for (int i = 0; i < programs.size(); ++i) {
        QSharedPointer<Program> program = programs[i];
        if (!program)
            continue;

        QDomElement child = doc.createElement(programTag);
        child.setAttribute(idAttr, QString::number(program->id()));

        switch (program->type()) {
        case TimerControlType:
            child.setAttribute(typeAttr, ProgramTypeConv<TimerControlType>::toString());
            break;
        case RelayControlType:
            child.setAttribute(typeAttr, ProgramTypeConv<RelayControlType>::toString());
            break;
        case PidControlType:
            child.setAttribute(typeAttr, ProgramTypeConv<PidControlType>::toString());
            break;
        case ButtonControlType:
            child.setAttribute(typeAttr, ProgramTypeConv<ButtonControlType>::toString());            
            break;
        case EmptyProgramType:
        default:
            child.setAttribute(typeAttr, ProgramTypeConv<EmptyProgramType>::toString());
            break;
        }

        program->providers()->xmlProvider()->toElement(child);

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
            QString type = child.attribute(typeAttr);
            if (type.isEmpty())
                continue;

            QSharedPointer<Program> program;
            if (type == ProgramTypeConv<TimerControlType>::toString())
                program = ProgramManager::createTimerControlProgram(id);
            else if (type == ProgramTypeConv<RelayControlType>::toString())
                program = ProgramManager::createRelayControlProgram(id);
            else if (type == ProgramTypeConv<PidControlType>::toString())
                program = ProgramManager::createPidControlProgram(id);
            else if (type == ProgramTypeConv<ButtonControlType>::toString())
                program = ProgramManager::createButtonControlProgram(id);
            else {
                program = ProgramManager::createEmptyProgram(id);
            }

            program->providers()->xmlProvider()->fromElement(child);

            programs.append(program);
        }

        child = child.nextSiblingElement();
    }

    return programs;
}
