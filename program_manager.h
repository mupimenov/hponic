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

    static QSharedPointer<Program> createEmptyProgram(int id);
    static QSharedPointer<Program> createTimerControlProgram(int id);
    static QSharedPointer<Program> createRelayControlProgram(int id);
    static QSharedPointer<Program> createPidControlProgram(int id);
    static QSharedPointer<Program> createButtonControlProgram(int id);

    int programCount() const;
    QSharedPointer<Program> program(int num) const;
    QSharedPointer<Program> programById(int id) const;
    const QList<QSharedPointer<Program> > &programs() const;

    enum {
        NoProgram = -1
    };

    int programNumber(Program *program) const;

    int addProgram(QSharedPointer<Program> program);
    void removeProgram(int num);
    void replaceProgram(int num, QSharedPointer<Program> program, bool keepname = false);

    void replacePrograms(const QList<QSharedPointer<Program> > &programs, bool keepname = false);

Q_SIGNALS:
    void programAdded(int num);
    void programReplaced(int num);
    void programUpdated(int num);
    void programRemoved(int num);

public Q_SLOTS:

private Q_SLOTS:
    void onProgramUpdated(Program *program);

private:
    QList<QSharedPointer<Program> > d_programs;
};

#endif // PROGRAMMANAGER_H
