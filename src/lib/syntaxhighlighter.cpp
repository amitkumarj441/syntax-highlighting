/*
    Copyright (C) 2016 Volker Krause <vkrause@kde.org>

    This program is free software; you can redistribute it and/or modify it
    under the terms of the GNU Library General Public License as published by
    the Free Software Foundation; either version 2 of the License, or (at your
    option) any later version.

    This program is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Library General Public
    License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "syntaxhighlighter.h"
#include "definition.h"
#include "format.h"
#include "state.h"
#include "theme.h"

#include <QDebug>

Q_DECLARE_METATYPE(QTextBlock)

using namespace SyntaxHighlighting;

namespace SyntaxHighlighting {
class TextBlockUserData : public QTextBlockUserData
{
public:
    explicit TextBlockUserData(const State &s) : state(s) {}
    State state;
};
}

SyntaxHighlighter::SyntaxHighlighter(QObject* parent) :
    QSyntaxHighlighter(parent)
{
    qRegisterMetaType<QTextBlock>();
}

SyntaxHighlighter::SyntaxHighlighter(QTextDocument *document) :
    QSyntaxHighlighter(document)
{
    qRegisterMetaType<QTextBlock>();
}

SyntaxHighlighter::~SyntaxHighlighter()
{
}

void SyntaxHighlighter::setDefinition(const Definition& def)
{
    const auto needsRehighlight = definition() != def;
    AbstractHighlighter::setDefinition(def);
    if (needsRehighlight)
        rehighlight();
}

void SyntaxHighlighter::highlightBlock(const QString& text)
{
    State state;
    if (currentBlock().position() > 0) {
        const auto prevBlock = currentBlock().previous();
        const auto prevData = dynamic_cast<TextBlockUserData*>(prevBlock.userData());
        if (prevData)
            state = prevData->state;
    }
    state = highlightLine(text, state);

    const auto data = dynamic_cast<TextBlockUserData*>(currentBlockUserData());
    if (!data) { // first time we highlight this
        setCurrentBlockUserData(new TextBlockUserData(state));
        return;
    }

    if (data->state == state) // we ended up in the same state, so we are done here
        return;
    data->state = state;

    const auto nextBlock = currentBlock().next();
    if (nextBlock.isValid())
        QMetaObject::invokeMethod(this, "rehighlightBlock", Qt::QueuedConnection, Q_ARG(QTextBlock, nextBlock));
}

void SyntaxHighlighter::applyFormat(int offset, int length, const SyntaxHighlighting::Format& format)
{
    if (format.isDefaultTextStyle(theme()) || length == 0)
        return;

    QTextCharFormat tf;
    if (format.hasTextColor(theme()))
        tf.setForeground(format.textColor(theme()));
    if (format.hasBackgroundColor(theme()))
        tf.setBackground(format.backgroundColor(theme()));

    if (format.isBold(theme()))
        tf.setFontWeight(QFont::Bold);
    if (format.isItalic(theme()))
        tf.setFontItalic(true);
    if (format.isUnderline(theme()))
        tf.setFontUnderline(true);
    if (format.isStrikeThrough(theme()))
        tf.setFontStrikeOut(true);

    QSyntaxHighlighter::setFormat(offset, length, tf);
}
