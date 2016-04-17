#ifndef PROGRAM_H
#define PROGRAM_H

#include <QObject>
#include <QDateTime>

#include <QSharedPointer>

enum ProgramType
{
    EmptyProgramType,

    TimerControlType,
    RelayControlType,
    PidControlType
};

class Program : public QObject
{
    Q_OBJECT
public:
    explicit Program(int __id, int __type, QObject *parent = 0);
    virtual ~Program();

    void setName(const QString &__name);
    const QString &name() const;

    int id() const;
    int type() const;

Q_SIGNALS:
    void changed(Program *sender = 0);

public Q_SLOTS:

protected:
    int d_id;
    int d_type;
    QString d_name;
};

/*
 * EmptyProgram
 * */

class EmptyProgram : public Program
{
    Q_OBJECT
public:
    explicit EmptyProgram(int __id, QObject *parent = 0);
    virtual ~EmptyProgram();

Q_SIGNALS:

public Q_SLOTS:

};

struct Cyclogram
{
    enum Type {
        Simple,
        Impulse
    };

    Type type;
    int count;
    int impulseDuration;
    int pauseDuration;

    Cyclogram();
    virtual ~Cyclogram();
};

enum TimeConstraints
{
    ALL_TIME = 0,
    STRICT_EQUALITY,
    EVERY_DAY,
    EVERY_MONTH,
    EVERY_YEAR
};

/*
 * TimerControlProgram
 * */

class TimerControlProgram : public Program
{
    Q_OBJECT
public:
    explicit TimerControlProgram(int __id, QObject *parent = 0);
    virtual ~TimerControlProgram();

    void setConstrains(int constrains);
    int constrains(void) const;

    void setFrom(const QDateTime &__from);
    const QDateTime &from() const;

    void setTo(const QDateTime &__to);
    const QDateTime &to() const;

    void setCyclogram(const Cyclogram &__cyclogram);
    const Cyclogram &cyclogram() const;

    void setOutput(int __id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:    
    int d_constrains;
    QDateTime d_from;
    QDateTime d_to;
    Cyclogram d_cyclogram;
    int d_output;
};

/*
 * RelayControlProgram
 * */

class RelayControlProgram : public Program
{
    Q_OBJECT
public:
    explicit RelayControlProgram(int __id, QObject *parent = 0);
    virtual ~RelayControlProgram();

    void setInput(int __id);
    int input() const;

    void setConstrains(int constrains);
    int constrains(void) const;

    void setFrom(const QDateTime &__from);
    const QDateTime &from() const;

    void setTo(const QDateTime &__to);
    const QDateTime &to() const;

    void setLowBound(float __lowBound);
    float lowBound() const;

    void setHighBound(float __highBound);
    float highBound() const;

    void setCyclogram(const Cyclogram &__cyclogram);
    const Cyclogram &cyclogram() const;

    void setOutput(int __id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_input;

    int d_constrains;
    QDateTime d_from;
    QDateTime d_to;
    float d_lowBound;
    float d_highBound;
    Cyclogram d_cyclogram;

    int d_output;
};

/*
 * PidControlProgram
 * */

class PidControlProgram : public Program
{
    Q_OBJECT
public:
    explicit PidControlProgram(int __id, QObject *parent = 0);
    virtual ~PidControlProgram();

    void setInput(int __id);
    int input() const;

    void setConstrains(int constrains);
    int constrains(void) const;

    void setFrom(const QDateTime &__from);
    const QDateTime &from() const;

    void setTo(const QDateTime &__to);
    const QDateTime &to() const;

    void setProportional(float __p);
    float proportional() const;

    void setIntegral(float __i);
    float integral() const;

    void setDifferential(float __d);
    float differential() const;

    void setOutput(int __id);
    int output() const;

Q_SIGNALS:

public Q_SLOTS:

private:
    int d_input;

    int d_constrains;
    QDateTime d_from;
    QDateTime d_to;

    float d_proportional;
    float d_integral;
    float d_differential;

    int d_output;
};

/*
 * ProgramConv
 * */

class ProgramConv
{
public:
    template<class P>
    static QSharedPointer<P> toProgram(QSharedPointer<Program> &program) {
        QSharedPointer<P> p = program.dynamicCast<P>();
        return p;
    }
};

#endif // PROGRAM_H
