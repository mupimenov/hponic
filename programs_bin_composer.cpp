#include "programs_bin_composer.h"

#include "program_manager.h"
#include "program_providers.h"

ProgramsBinComposer::ProgramsBinComposer(QObject *parent) :
    QObject(parent) {}


ProgramsBinComposerV1::ProgramsBinComposerV1(QObject *parent) :
    ProgramsBinComposer(parent) {}

QByteArray ProgramsBinComposerV1::toArray(const QList<QSharedPointer<Program> > &programs)
{
    QByteArray arr;

    for (int i = 0; i < programs.size(); ++i) {
        QSharedPointer<Program> program = programs[i];
        if (!program)
            continue;

        arr.append(program->providers()->binProvider()->toArray());
    }

    return arr;
}

QList<QSharedPointer<Program> > ProgramsBinComposerV1::fromArray(const QByteArray &arr)
{
    const int chunk = 48;

    QList<QSharedPointer<Program> > programs;
    for (int offset = 0; offset + chunk <= arr.size(); offset += chunk) {
        QByteArray progArr = arr.mid(offset, chunk);
        QSharedPointer<Program> program;

        switch (progArr.at(0)) {
        case TimerControlType:
            program = ProgramManager::createTimerControlProgram(progArr[1]);
            break;
        case RelayControlType:
            program = ProgramManager::createRelayControlProgram(progArr[1]);
            break;
        case PidControlType:
            program = ProgramManager::createPidControlProgram(progArr[1]);
            break;
        case ButtonControlType:
            program = ProgramManager::createButtonControlProgram(progArr[1]);
            break;
        case EmptyProgramType:
        default:
            program = ProgramManager::createEmptyProgram(progArr[1]);
            break;
        }

        program->providers()->binProvider()->fromArray(progArr);

        programs.append(program);
    }

    return programs;
}
