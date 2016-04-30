#include "programs_bin_composer.h"

ProgramsBinComposer::ProgramsBinComposer(QObject *parent) : QObject(parent)
{

}


ProgramsBinComposerV1::ProgramsBinComposerV1(QObject *parent) : ProgramsBinComposer(parent)
{

}

static void dateTimeToArray(const QDateTime &dt, QByteArray &slot, int offset)
{
    slot[offset] = dt.time().second();
    slot[offset + 1] = dt.time().minute();
    slot[offset + 2] = dt.time().hour();
    slot[offset + 3] = dt.date().day();
    slot[offset + 4] = dt.date().month();
    slot[offset + 5] = dt.date().year() % 100;
}

static void cyclogramToArray(const Cyclogram &c, QByteArray &slot, int offset)
{
    slot[offset] = c.type == Cyclogram::Simple? 0: 1;

    switch (c.type)
    {
    case Cyclogram::Simple:
        break;
    case Cyclogram::Impulse:
        slot[offset + 1] = c.count & 0xFF;
        slot[offset + 2] = (c.count >> 8) & 0xFF;
        slot[offset + 3] = c.impulseDuration & 0xFF;
        slot[offset + 4] = (c.impulseDuration >> 8) & 0xFF;
        slot[offset + 5] = c.pauseDuration & 0xFF;
        slot[offset + 6] = (c.pauseDuration >> 8) & 0xFF;
        break;
    }
}

static const int slot_size = 48;

QByteArray ProgramsBinComposerV1::toArray(const QList<QSharedPointer<Program> > &programs)
{
    QByteArray arr;

    for (int i = 0; i < programs.size(); ++i) {
        QSharedPointer<Program> program = programs[i];
        if (!program)
            continue;

        QByteArray slot(slot_size, char(0));
        slot[0] = program->type();
        slot[1] = program->id();

        switch (program->type()) {
        case EmptyProgramType:
        {
            break;
        }
        case TimerControlType:
        {
            QSharedPointer<TimerControlProgram> timerControl = ProgramConv::toProgram<TimerControlProgram>(program);
            slot[2] = timerControl->constrains();                   // 1
            dateTimeToArray(timerControl->from(), slot, 3);         // 6
            dateTimeToArray(timerControl->to(), slot, 9);           // 6
            cyclogramToArray(timerControl->cyclogram(), slot, 15);  // 7
            slot[22] = timerControl->output();

            break;
        }
        case RelayControlType:
        {
            QSharedPointer<RelayControlProgram> relayControl = ProgramConv::toProgram<RelayControlProgram>(program);
            slot[2] = relayControl->input();                        // 1
            slot[3] = relayControl->constrains();                   // 1
            dateTimeToArray(relayControl->from(), slot, 4);         // 6
            dateTimeToArray(relayControl->to(), slot, 10);          // 6
            floatToArray(relayControl->lowBound(), slot, 16);       // 4
            floatToArray(relayControl->highBound(), slot, 20);      // 4
            cyclogramToArray(relayControl->cyclogram(), slot, 24);  // 7
            slot[31] = relayControl->inverse()? 0x01: 0x00;         // 1
            slot[32] = relayControl->output();                      // 1

            break;
        }
        case PidControlType:
        {
            QSharedPointer<PidControlProgram> pidControl = ProgramConv::toProgram<PidControlProgram>(program);
            slot[2] = pidControl->input();                        // 1
            slot[3] = pidControl->constrains();                   // 1
            dateTimeToArray(pidControl->from(), slot, 4);         // 6
            dateTimeToArray(pidControl->to(), slot, 10);          // 6
            floatToArray(pidControl->desired(), slot, 16);        // 4
            floatToArray(pidControl->proportional(), slot, 20);   // 4
            floatToArray(pidControl->integral(), slot, 24);       // 4
            floatToArray(pidControl->differential(), slot, 28);   // 4
            slot[32] = pidControl->inverse()? 0x01: 0x00;         // 1
            slot[33] = pidControl->output();                      // 1

            break;
        }

        default:
            break;
        }

        arr.append(slot);
    }

    return arr;
}

static QDateTime arrayToDateTime(QByteArray &slot, int offset)
{
    QDate d(slot.at(offset + 5), slot.at(offset + 4), slot.at(offset + 3));
    QTime t(slot.at(offset + 2), slot.at(offset + 1), slot.at(offset));

    return QDateTime(d, t);
}

static Cyclogram arrayToCyclogram(QByteArray &slot, int offset)
{
    Cyclogram c;
    c.type = static_cast<Cyclogram::Type>(slot.at(offset));
    if (c.type != Cyclogram::Simple && c.type != Cyclogram::Impulse)
        c.type = Cyclogram::Simple;

    switch (c.type)
    {
    case Cyclogram::Simple:
        break;
    case Cyclogram::Impulse:
        c.count = (quint8)slot.at(offset + 1) + ((quint8)slot.at(offset + 2) << 8);
        c.impulseDuration = (quint8)slot.at(offset + 3) + ((quint8)slot.at(offset + 4) << 8);
        c.pauseDuration = (quint8)slot.at(offset + 5) + ((quint8)slot.at(offset + 6) << 8);
        break;
    }

    return c;
}

QList<QSharedPointer<Program> > ProgramsBinComposerV1::fromArray(const QByteArray &arr)
{
    QList<QSharedPointer<Program> > programs;
    for (int offset = 0; offset + slot_size <= arr.size(); offset += slot_size) {
        QByteArray slot = arr.mid(offset, slot_size);

        switch (slot.at(0)) {
        case EmptyProgramType:
        {
            EmptyProgram *emptyProgram = new EmptyProgram(slot.at(1));
            programs.append(QSharedPointer<Program>(emptyProgram));
            break;
        }
        case TimerControlType:
        {
            TimerControlProgram *timerControl = new TimerControlProgram(slot.at(1));
            timerControl->setConstrains(slot.at(2));    // 1
            QDateTime dt = arrayToDateTime(slot, 3);    // 6
            timerControl->setFrom(dt);
            dt = arrayToDateTime(slot, 9);              // 6
            timerControl->setTo(dt);

            Cyclogram c = arrayToCyclogram(slot, 15);   // 7
            timerControl->setCyclogram(c);

            timerControl->setOutput(slot.at(22));       // 1

            programs.append(QSharedPointer<Program>(timerControl));
            break;
        }
        case RelayControlType:
        {
            RelayControlProgram *relayControl = new RelayControlProgram(slot.at(1));
            relayControl->setInput(slot.at(2));         // 1
            relayControl->setConstrains(slot.at(3));    // 1
            QDateTime dt = arrayToDateTime(slot, 4);    // 6
            relayControl->setFrom(dt);
            dt = arrayToDateTime(slot, 10);             // 6
            relayControl->setTo(dt);

            float f = arrayToFloat(slot, 16);           // 4
            relayControl->setLowBound(f);
            f = arrayToFloat(slot, 20);                 // 4
            relayControl->setHighBound(f);

            Cyclogram c = arrayToCyclogram(slot, 24);   // 7
            relayControl->setCyclogram(c);

            relayControl->setInverse(slot.at(31) == 0x00);  // 1
            relayControl->setOutput(slot.at(32));           // 1

            programs.append(QSharedPointer<Program>(relayControl));
            break;
        }
        case PidControlType:
        {
            PidControlProgram *pidControl = new PidControlProgram(slot.at(1));
            pidControl->setInput(slot.at(2));           // 1
            pidControl->setConstrains(slot.at(3));      // 1
            QDateTime dt = arrayToDateTime(slot, 4);    // 6
            pidControl->setFrom(dt);
            dt = arrayToDateTime(slot, 10);             // 6
            pidControl->setTo(dt);

            float f = arrayToFloat(slot, 16);           // 4
            pidControl->setDesired(f);
            f = arrayToFloat(slot, 20);                 // 4
            pidControl->setProportional(f);
            f = arrayToFloat(slot, 24);                 // 4
            pidControl->setIntegral(f);
            f = arrayToFloat(slot, 28);                 // 4
            pidControl->setDifferential(f);

            pidControl->setInverse(slot.at(32) == 0x01);    // 1
            pidControl->setOutput(slot.at(33));             // 1

            programs.append(QSharedPointer<Program>(pidControl));
            break;
        }
        default:
            break;
        }
    }

    return programs;
}
