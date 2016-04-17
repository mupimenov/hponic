#include "program_manager.h"

ProgramManager::ProgramManager(QObject *parent) : QObject(parent)
{

}

int ProgramManager::programCount() const
{
    return d_programs.size();
}

QSharedPointer<Program> ProgramManager::program(int __num) const
{
    if (__num < 0 || __num >= d_programs.size())
        return QSharedPointer<Program>();

    return d_programs[__num];
}

QSharedPointer<Program> ProgramManager::programById(int __id) const
{
    QList<QSharedPointer<Program> >::const_iterator it = d_programs.begin();
    for (; it != d_programs.end(); ++it) {
        if ((*it)->id() == __id)
            return (*it);
    }

    return QSharedPointer<Program>();
}

const QList<QSharedPointer<Program> > &ProgramManager::programs() const
{
    return d_programs;
}

int ProgramManager::programNumber(Program *__program) const
{
    QList<QSharedPointer<Program> >::const_iterator it = d_programs.begin();
    for (int j = 0; it != d_programs.end(); ++it, ++j) {
        if ((*it).data() == __program)
            return j;
    }

    return NoProgram;
}

int ProgramManager::addProgram(QSharedPointer<Program> __program)
{
    d_programs.append(__program);

    connect(__program.data(), SIGNAL(changed(Program*)), this, SLOT(onProgramUpdated(Program*)), Qt::DirectConnection);
    int num = d_programs.size() - 1;
    Q_EMIT programAdded(num);

    return num;
}

void ProgramManager::removeProgram(int __num)
{
    d_programs.removeAt(__num);
    Q_EMIT programRemoved(__num);
}

void ProgramManager::updateProgram(int __num, QSharedPointer<Program> __program)
{
    d_programs.replace(__num, __program);
    connect(__program.data(), SIGNAL(changed(Program*)), this, SLOT(onProgramUpdated(Program*)), Qt::DirectConnection);
    Q_EMIT programUpdated(__num);
}

void ProgramManager::updatePrograms(const QList<QSharedPointer<Program> > &programs)
{
    QList<QSharedPointer<Program> >::const_iterator it = programs.begin();
    for (int j = 0; it != programs.end(); ++it, ++j)
        updateProgram(j, *it);
}

void ProgramManager::onProgramUpdated(Program *__program)
{
    int num = programNumber(__program);
    if (num == NoProgram)
        return;

    Q_EMIT programUpdated(num);
}
