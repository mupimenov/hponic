#include "ioslot_manager.h"

#include "ioslot_providers.h"

IoslotManager::IoslotManager(QObject *parent) : QObject(parent)
{

}

QSharedPointer<Ioslot> IoslotManager::createEmptySlot(int id)
{
    QSharedPointer<EmptySlot> slot(new EmptySlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new EmptySlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new EmptySlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new EmptySlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createAnalogInputSlot(int id)
{
    QSharedPointer<AnalogInputSlot> slot(new AnalogInputSlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new AnalogInputSlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new AnalogInputSlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new AnalogInputSlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createDiscreteInputSlot(int id)
{
    QSharedPointer<DiscreteInputSlot> slot(new DiscreteInputSlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new DiscreteInputSlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new DiscreteInputSlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new DiscreteInputSlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createDiscreteOutputSlot(int id)
{
    QSharedPointer<DiscreteOutputSlot> slot(new DiscreteOutputSlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new DiscreteOutputSlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new DiscreteOutputSlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new DiscreteOutputSlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createDHTxxSlot(int id)
{
    QSharedPointer<DHTxxSlot> slot(new DHTxxSlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new DHTxxSlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new DHTxxSlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new DHTxxSlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createDallasTemperatureSlot(int id)
{
    QSharedPointer<DallasTemperatureSlot> slot(new DallasTemperatureSlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new DallasTemperatureSlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new DallasTemperatureSlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new DallasTemperatureSlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createMHZ19Slot(int id)
{
    QSharedPointer<MHZ19Slot> slot(new MHZ19Slot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new MHZ19SlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new MHZ19SlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new MHZ19SlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

QSharedPointer<Ioslot> IoslotManager::createSHT2xSlot(int id)
{
    QSharedPointer<SHT2xSlot> slot(new SHT2xSlot(id));
    QSharedPointer<IoslotProviders> providers(new IoslotProvidersV1(
                                                  QSharedPointer<IoslotEditorProvider>(
                                                      new SHT2xSlotEditorProvider(slot)),
                                                  QSharedPointer<IoslotBinProvider>(
                                                      new SHT2xSlotBinProvider(slot)),
                                                  QSharedPointer<IoslotXmlProvider>(
                                                      new SHT2xSlotXmlProvider(slot))));
    slot->setProviders(providers);
    return slot.dynamicCast<Ioslot>();
}

int IoslotManager::ioslotCount() const
{
    return d_ioslots.size();
}

QSharedPointer<Ioslot> IoslotManager::ioslot(int num) const
{
    if (num < 0 || num >= d_ioslots.size())
        return QSharedPointer<Ioslot>();

    return d_ioslots[num];
}

const QList<QSharedPointer<Ioslot> > &IoslotManager::ioslots() const
{
    return d_ioslots;
}

int IoslotManager::ioslotNumber(Ioslot * ioslot) const
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (int j = 0; it != d_ioslots.end(); ++it, ++j) {
        if ((*it).data() == ioslot)
            return j;
    }

    return NoIoslot;
}

QSharedPointer<Ioslot> IoslotManager::ioslotById(int id) const
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (; it != d_ioslots.end(); ++it) {
        if ((*it)->id() == id)
            return (*it);
    }

    return QSharedPointer<Ioslot>();
}

QList<QSharedPointer<Ioslot> > IoslotManager::ioslotsByType(const QList<int> &types) const
{
    QList<QSharedPointer<Ioslot> > list;
    QList<QSharedPointer<Ioslot> >::const_iterator it = d_ioslots.begin();
    for (; it != d_ioslots.end(); ++it) {
        if (types.contains((*it)->type()))
            list.append(*it);
    }

    return list;
}

int IoslotManager::addIoslot(QSharedPointer<Ioslot> ioslot)
{
    d_ioslots.append(ioslot);

    connect(ioslot.data(), SIGNAL(changed(Ioslot*)), this, SLOT(onIoslotChanged(Ioslot*)), Qt::DirectConnection);
    int num = d_ioslots.size() - 1;

    Q_EMIT ioslotAdded(num);
    return num;
}

void IoslotManager::removeIoslot(int num)
{
    d_ioslots.removeAt(num);
    Q_EMIT ioslotRemoved(num);
}

void IoslotManager::replaceIoslot(int num, QSharedPointer<Ioslot> ioslot, bool keepname)
{
    if (keepname) {
        QSharedPointer<Ioslot> current = this->ioslot(num);
        if (current->driver() == ioslot->driver())
            ioslot->setName(current->name());
    }

    d_ioslots.replace(num, ioslot);
    connect(ioslot.data(), SIGNAL(changed(Ioslot*)), this, SLOT(onIoslotChanged(Ioslot*)), Qt::DirectConnection);
    Q_EMIT ioslotReplaced(num);
}

void IoslotManager::onIoslotChanged(Ioslot *slot)
{
    int num = ioslotNumber(slot);
    if (num == NoIoslot)
        return;

    Q_EMIT ioslotUpdated(num);
}

void IoslotManager::replaceIoslots(const QList<QSharedPointer<Ioslot> > &ioslots, bool keepname)
{
    QList<QSharedPointer<Ioslot> >::const_iterator it = ioslots.begin();
    for (int j = 0; it != ioslots.end(); ++it, ++j)
        replaceIoslot(j, *it, keepname);
}
