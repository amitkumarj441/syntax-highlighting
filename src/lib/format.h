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

#ifndef SYNTAXHIGHLIGHTING_FORMAT_H
#define SYNTAXHIGHLIGHTING_FORMAT_H

#include "kf5syntaxhighlighting_export.h"

#include <QTypeInfo>

#include <memory>

class QColor;
class QString;
class QXmlStreamReader;

namespace SyntaxHighlighting {

class DefinitionRef;
class FormatPrivate;
class Theme;

/** Describes the format to be used for a specific text fragment.
 *  The actual format used for displaying is merged from the format information
 *  in the syntax definition file, and a theme.
 *
 *  @see Theme
 */
class KF5SYNTAXHIGHLIGHTING_EXPORT Format
{
public:
    /** Creates an empty/invalid format. */
    Format();
    ~Format();

    /** Returns @c true if this is a valid format, ie. one that
     *  was read from a syntax definition file.
     */
    bool isValid() const;

    /** The name of this format as used in the syntax definition file. */
    QString name() const;

    /** Returns @c true if the combination of this format and the theme @p theme
     *  do not change the default text format in any way.
     *  This is useful for output formats where changing formatting implies cost,
     *  and thus benefit from optimizing the default case of not having any format
     *  applied. If you make use of this, make sure to set the default text style
     *  to what the corresponding theme sets for Theme::Normal.
     */
    bool isDefaultTextStyle(const Theme &theme) const;

    /** Returns @c true if the combination of this format and the theme @p theme
     *  change the foreground color compared to the default format.
     */
    bool hasTextColor(const Theme &theme) const;
    /** Returns the foreground color of the combination of this format and the
     *  given theme.
     */
    QColor textColor(const Theme &theme) const;
    /** Returns the foreground color for selected text of the combination of
     *  this format and the given theme.
     */
    QColor selectedTextColor(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the theme @p theme
     *  change the background color compared to the default format.
     */
    bool hasBackgroundColor(const Theme &theme) const;
    /** Returns the background color of the combination of this format and the
     *  given theme.
     */
    QColor backgroundColor(const Theme &theme) const;
    /** Returns the background color of selected text of the combination of
     *  this format and the given theme.
     */
    QColor selectedBackgroundColor(const Theme &theme) const;

    /** Returns @c true if the combination of this format and the given theme
     *  results in bold text formatting.
     */
    bool isBold(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the given theme
     *  results in italic text formatting.
     */
    bool isItalic(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the given theme
     *  results in underlined text.
     */
    bool isUnderline(const Theme &theme) const;
    /** Returns @c true if the combination of this format and the given theme
     *  results in struck through text.
     */
    bool isStrikeThrough(const Theme &theme) const;

    /**
     * Returns whether characters with this format should be spell checked.
     */
    bool spellCheck() const;

private:
    friend class DefinitionData;
    void setDefinition(const DefinitionRef &def);
    void load(QXmlStreamReader &reader);
    std::shared_ptr<FormatPrivate> d;
};
}

Q_DECLARE_TYPEINFO(SyntaxHighlighting::Format, Q_MOVABLE_TYPE);

#endif // SYNTAXHIGHLIGHTING_FORMAT_H
