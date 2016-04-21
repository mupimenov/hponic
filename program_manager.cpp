#include "program_manager.h"

ProgramManager::ProgramManager(QObject *parent) : QObject(parent)
{

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
