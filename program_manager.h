#ifndef PROGRAMMANAGER_H
#define PROGRAMMANAGER_H

#include <QObject>
#include <QList>

#include <QSharedPointer>

#include "program.h"

class ProgramManager : public QObject
{
    Q_OBJECT
public:
    explicit ProgramManager(QObject *parent = 0);

    int programCount() const;
    QSharedPointer<Program> program(int __num) const;
    QSharedPointer<Program> programById(int __id) const;
    const QList<QSharedPointer<Program> > &programs() const;

    enum {
        NoProgram = -1
    };

    int programNumber(Program *__program) const;

    int addProgram(QSharedPointer<Program> __program);
    void removeProgram(int __num);
    void updateProgram(int __num, QSharedPointer<Program> __program);

    void updatePrograms(const QList<QSharedPointer<Program> > &programs);

Q_SIGNALS:
    void programAdded(int __num);
    void programUpdated(int __num);
    void programRemoved(int __num);

public Q_SLOTS:

private Q_SLOTS:
    void onProgramUpdated(Program *__program);

private:
    QList<QSharedPointer<Program> > d_programs;
};

#endif // PROGRAMMANAGER_H
