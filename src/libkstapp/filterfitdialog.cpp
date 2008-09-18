/***************************************************************************
 *                                                                         *
 *   copyright : (C) 2007 The University of Toronto                        *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "filterfitdialog.h"

#include "dialogpage.h"

#include "datacollection.h"
#include "document.h"
#include "basicplugin.h"

namespace Kst {

FilterFitTab::FilterFitTab(QString& pluginName, QWidget *parent)
  : DataTab(parent), _configWidget(0), _layout(0), _store(0) {

  setupUi(this);
  setTabTitle(tr("Plugin"));

  _type = (DataObjectPluginInterface::PluginTypeID)DataObject::pluginType(pluginName);
  if (_type == DataObjectPluginInterface::Filter) {
    _pluginCombo->addItems(DataObject::filterPluginList());
  } else if (_type == DataObjectPluginInterface::Fit) {
    _pluginCombo->addItems(DataObject::fitsPluginList());
  }

  connect(_pluginCombo, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(pluginChanged(const QString&)));
  _curveAppearance->setVisible(false);

  pluginChanged(pluginName);
}


FilterFitTab::~FilterFitTab() {
}


void FilterFitTab::setObjectStore(ObjectStore *store) {
  _store = store;
   _configWidget->setObjectStore(store);
}


void FilterFitTab::pluginChanged(const QString &plugin) {
  if (plugin != _pluginCombo->currentText()) {
    _pluginCombo->setCurrentIndex(_pluginCombo->findText(plugin));
  }

  _pluginLabel->setText(plugin);
  _descriptionLabel->setText(DataObject::pluginDescription(plugin));

  if (_layout) {
    delete _layout;
  }
  if (_configWidget) {
    delete _configWidget;
  }
  _layout = new QGridLayout(_inputOutputBox);
  _configWidget = DataObject::pluginWidget(plugin);
  _configWidget->setupSlots(this);
  if (_store) {
    _configWidget->setObjectStore(_store);
  }
  _layout->addWidget(_configWidget, 0, 0);
  _layout->activate();
}


FilterFitDialog::FilterFitDialog(QString& pluginName, ObjectPtr dataObject, QWidget *parent)
  : DataDialog(dataObject, parent), _pluginName(pluginName) {

  if (editMode() == Edit)
    setWindowTitle(tr("Edit Plugin"));
  else
    setWindowTitle(tr("New Plugin"));

  _filterFitTab = new FilterFitTab(pluginName, this);
  addDataTab(_filterFitTab);

  if (dataObject) {
    _filterFitTab->configWidget()->setupFromObject(dataObject);
  } else {
    _filterFitTab->configWidget()->load();
  }
}


FilterFitDialog::~FilterFitDialog() {
}


QString FilterFitDialog::tagString() const {
  return DataDialog::tagString();
}


ObjectPtr FilterFitDialog::createNewDataObject() {
  DataObjectPtr dataObject = DataObject::createPlugin(_pluginName, _document->objectStore(), _filterFitTab->configWidget());
  _filterFitTab->configWidget()->save();
  return dataObject;
}


ObjectPtr FilterFitDialog::editExistingDataObject() const {
  if (BasicPlugin* plugin = kst_cast<BasicPlugin>(dataObject())) {
    plugin->writeLock();
    plugin->change(_filterFitTab->configWidget());
    plugin->update();
    plugin->unlock();
  }
  return dataObject();
}

}

// vim: ts=2 sw=2 et
