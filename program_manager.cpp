#include "program_manager.h"

#include "program_providers.h"

ProgramManager::ProgramManager(QObject *parent) :
    QObject(parent) {}

QSharedPointer<Program> ProgramManager::createEmptyProgram(int id)
{
    QSharedPointer<EmptyProgram> program(new EmptyProgram(id));
    QSharedPointer<ProgramProviders> providers(new ProgramProvidersV1(
                                                   QSharedPointer<ProgramEditorProvider>(
                                                       new EmptyProgramEditorProvider(program)),
                                                   QSharedPointer<ProgramBinProvider>(
                                                       new EmptyProgramBinProvider(program)),
                                                   QSharedPointer<ProgramXmlProvider>(
                                                       new EmptyProgramXmlProvider(program))));
    program->setProviders(providers);
    return program.dynamicCast<Program>();
}

QSharedPointer<Program> ProgramManager::createTimerControlProgram(int id)
{
    QSharedPointer<TimerControlProgram> program(new TimerControlProgram(id));
    QSharedPointer<ProgramProviders> providers(new ProgramProvidersV1(
                                                   QSharedPointer<ProgramEditorProvider>(
                                                       new TimerControlProgramEditorProvider(program)),
                                                   QSharedPointer<ProgramBinProvider>(
                                                       new TimerControlProgramBinProvider(program)),
                                                   QSharedPointer<ProgramXmlProvider>(
                                                       new TimerControlProgramXmlProvider(program))));
    program->setProviders(providers);
    return program.dynamicCast<Program>();
}

QSharedPointer<Program> ProgramManager::createRelayControlProgram(int id)
{
    QSharedPointer<RelayControlProgram> program(new RelayControlProgram(id));
    QSharedPointer<ProgramProviders> providers(new ProgramProvidersV1(
                                                   QSharedPointer<ProgramEditorProvider>(
                                                       new RelayControlProgramEditorProvider(program)),
                                                   QSharedPointer<ProgramBinProvider>(
                                                       new RelayControlProgramBinProvider(program)),
                                                   QSharedPointer<ProgramXmlProvider>(
                                                       new RelayControlProgramXmlProvider(program))));
    program->setProviders(providers);
    return program.dynamicCast<Program>();
}

QSharedPointer<Program> ProgramManager::createPidControlProgram(int id)
{
    QSharedPointer<PidControlProgram> program(new PidControlProgram(id));
    QSharedPointer<ProgramProviders> providers(new ProgramProvidersV1(
                                                   QSharedPointer<ProgramEditorProvider>(
                                                       new PidControlProgramEditorProvider(program)),
                                                   QSharedPointer<ProgramBinProvider>(
                                                       new PidControlProgramBinProvider(program)),
                                                   QSharedPointer<ProgramXmlProvider>(
                                                       new PidControlProgramXmlProvider(program))));
    program->setProviders(providers);
    return program.dynamicCast<Program>();
}

QSharedPointer<Program> ProgramManager::createButtonControlProgram(int id)
{
    QSharedPointer<ButtonControlProgram> program(new ButtonControlProgram(id));
    QSharedPointer<ProgramProviders> providers(new ProgramProvidersV1(
                                                   QSharedPointer<ProgramEditorProvider>(
                                                       new ButtonControlProgramEditorProvider(program)),
                                                   QSharedPointer<ProgramBinProvider>(
                                                       new ButtonControlProgramBinProvider(program)),
                                                   QSharedPointer<ProgramXmlProvider>(
                                                       new ButtonControlProgramXmlProvider(program))));
    program->setProviders(providers);
    return program.dynamicCast<Program>();
}

int ProgramManager::programCount() const
{
    return d_programs.size();
}

QSharedPointer<Program> ProgramManager::program(int num) const
{
    if (num < 0 || num >= d_programs.size())
        return QSharedPointer<Program>();

    return d_programs[num];
}

QSharedPointer<Program> ProgramManager::programById(int id) const
{
    QList<QSharedPointer<Program> >::const_iterator it = d_programs.begin();
    for (; it != d_programs.end(); ++it) {
        if ((*it)->id() == id)
            return (*it);
    }

    return QSharedPointer<Program>();
}

const QList<QSharedPointer<Program> > &ProgramManager::programs() const
{
    return d_programs;
}

int ProgramManager::programNumber(Program *program) const
{
    QList<QSharedPointer<Program> >::const_iterator it = d_programs.begin();
    for (int j = 0; it != d_programs.end(); ++it, ++j) {
        if ((*it).data() == program)
            return j;
    }

    return NoProgram;
}

int ProgramManager::addProgram(QSharedPointer<Program> program)
{
    d_programs.append(program);

    connect(program.data(), SIGNAL(changed(Program*)), this, SLOT(onProgramUpdated(Program*)), Qt::DirectConnection);
    int num = d_programs.size() - 1;
    Q_EMIT programAdded(num);

    return num;
}

void ProgramManager::removeProgram(int num)
{
    d_programs.removeAt(num);
    Q_EMIT programRemoved(num);
}

void ProgramManager::replaceProgram(int num, QSharedPointer<Program> program, bool keepname)
{
    if (keepname) {
        QSharedPointer<Program> current = this->program(num);
        if (current->type() == program->type())
            program->setName(current->name());
    }

    d_programs.replace(num, program);
    connect(program.data(), SIGNAL(changed(Program*)), this, SLOT(onProgramUpdated(Program*)), Qt::DirectConnection);
    Q_EMIT programReplaced(num);
}

void ProgramManager::replacePrograms(const QList<QSharedPointer<Program> > &programs, bool keepname)
{
    QList<QSharedPointer<Program> >::const_iterator it = programs.begin();
    for (int j = 0; it != programs.end(); ++it, ++j)
        replaceProgram(j, *it, keepname);
}

void ProgramManager::onProgramUpdated(Program *program)
{
    int num = programNumber(program);
    if (num == NoProgram)
        return;

    Q_EMIT programUpdated(num);
}
