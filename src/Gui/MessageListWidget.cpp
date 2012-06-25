/* Copyright (C) 2006 - 2012 Jan Kundrát <jkt@flaska.net>

   This file is part of the Trojita Qt IMAP e-mail client,
   http://trojita.flaska.net/

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2 of the License, or the version 3 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "MessageListWidget.h"
#include <QCheckBox>
#include <QLineEdit>
#include <QVBoxLayout>
#include "MsgListView.h"

#include <QDebug>

namespace Gui {

MessageListWidget::MessageListWidget(QWidget *parent) :
    QWidget(parent), m_supportsFuzzySearch(false)
{
    tree = new MsgListView(this);

    m_quickSearchText = new QLineEdit(this);
    m_quickSearchText->setPlaceholderText(tr("Quick Search..."));

    connect(m_quickSearchText, SIGNAL(returnPressed()), this, SLOT(slotApplySearch()));
    connect(m_quickSearchText, SIGNAL(textChanged(QString)), this, SLOT(slotAutoHideOptionsBar()));

    m_searchOptionsBar = new QWidget(this);

    // The option bar shall use a slightly smaller font size
    QFont f = font();
    f.setPointSizeF(f.pointSizeF() * 0.80);
    m_searchOptionsBar->setFont(f);

    m_searchFuzzy = new QCheckBox(tr("Fuzzy"), m_searchOptionsBar);
    m_searchInSubject = new QCheckBox(tr("Subject"), m_searchOptionsBar);
    m_searchInSubject->setChecked(true);
    m_searchInBody = new QCheckBox(tr("Body"), m_searchOptionsBar);
    m_searchInSenders = new QCheckBox(tr("Senders"), m_searchOptionsBar);
    m_searchInSenders->setChecked(true);
    m_searchInRecipients = new QCheckBox(tr("Recipients"), m_searchOptionsBar);

    QHBoxLayout *fieldsLayout = new QHBoxLayout(m_searchOptionsBar);
    fieldsLayout->addWidget(m_searchFuzzy);
    fieldsLayout->addStretch();
    fieldsLayout->addWidget(m_searchInSubject);
    fieldsLayout->addWidget(m_searchInBody);
    fieldsLayout->addWidget(m_searchInSenders);
    fieldsLayout->addWidget(m_searchInRecipients);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setSpacing(0);
    layout->addWidget(m_quickSearchText);
    layout->addWidget(m_searchOptionsBar);
    layout->addWidget(tree);

    slotAutoEnableDisableSearch();
}

void MessageListWidget::slotApplySearch()
{
    emit requestingSearch(searchConditions());
}

void MessageListWidget::slotAutoEnableDisableSearch()
{
    bool isEnabled;
    if (tree && tree->model()) {
        isEnabled = tree->model()->rowCount();
    } else {
        isEnabled = false;
    }
    m_quickSearchText->setEnabled(isEnabled);
    m_searchFuzzy->setEnabled(isEnabled && m_supportsFuzzySearch);
    m_searchInBody->setEnabled(isEnabled);
    m_searchInRecipients->setEnabled(isEnabled);
    m_searchInSenders->setEnabled(isEnabled);
    m_searchInSubject->setEnabled(isEnabled);
    slotAutoHideOptionsBar();
}

void MessageListWidget::slotAutoHideOptionsBar()
{
    m_searchOptionsBar->setVisible(!m_quickSearchText->text().isEmpty() && m_quickSearchText->isEnabled());
}

QStringList MessageListWidget::searchConditions() const
{
    if (!m_quickSearchText->isEnabled() || m_quickSearchText->text().isEmpty())
        return QStringList();

    QStringList keys;
    if (m_searchInSubject->isChecked())
        keys << QLatin1String("SUBJECT");
    if (m_searchInBody->isChecked())
        keys << QLatin1String("BODY");
    if (m_searchInRecipients->isChecked())
        keys << QLatin1String("TO") << QLatin1String("CC") << QLatin1String("BCC");
    if (m_searchInSenders->isChecked())
        keys << QLatin1String("FROM");

    if (keys.isEmpty())
        return keys;

    QStringList res;
    Q_FOREACH(const QString &key, keys) {
        if (m_supportsFuzzySearch)
            res << QLatin1String("FUZZY");
        res << key << m_quickSearchText->text();
    }
    if (keys.size() > 1)
        res.prepend(QLatin1String("OR"));

    return res;
}

void MessageListWidget::setFuzzySearchSupported(bool supported)
{
    m_supportsFuzzySearch = supported;
    m_searchFuzzy->setEnabled(supported);
    m_searchFuzzy->setChecked(supported);
}

}
