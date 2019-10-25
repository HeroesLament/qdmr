#include "csvreader.hh"
#include <QDebug>
#include "config.hh"

#include <QRegExp>

QVector< QPair<QRegExp, CSVLexer::Token::TokenType> > CSVLexer::_pattern = {
  { QRegExp("^([a-zA-Z_][a-zA-Z0-9_]*)"),      CSVLexer::Token::T_KEYWORD },
  { QRegExp("^\"([^\"\r\n]*)\""),              CSVLexer::Token::T_STRING },
  { QRegExp("^([+-]?[0-9]+(\\.[0-9]*)?)"),     CSVLexer::Token::T_NUMBER },
  { QRegExp("^(:)"),                           CSVLexer::Token::T_COLON },
  { QRegExp("^(-)"),                           CSVLexer::Token::T_NOT_SET },
  { QRegExp("^(\\+)"),                         CSVLexer::Token::T_ENABLED },
  { QRegExp("^(,)"),                           CSVLexer::Token::T_COMMA },
  { QRegExp("^([ \t]+)"),                      CSVLexer::Token::T_WHITESPACE },
  { QRegExp("^(\r?\n)"),                       CSVLexer::Token::T_NEWLINE},
  { QRegExp("^(#[^\n\r]*)"),                   CSVLexer::Token::T_COMMENT},
};


/* ********************************************************************************************* *
 * Implementation of CSVLexer
 * ********************************************************************************************* */
CSVLexer::CSVLexer(QTextStream &stream, QObject *parent)
  : QObject(parent), _stream(stream), _stack(), _currentLine()
{
  _stream.seek(0);
  _stack.reserve(10);
  _stack.push_back({0, 1, 1});
  _currentLine = _stream.readLine();
}

CSVLexer::Token
CSVLexer::next() {
  Token token = lex();
  while ((Token::T_WHITESPACE == token.type) || (Token::T_COMMENT == token.type))
    token = lex();
  return token;
}

CSVLexer::Token
CSVLexer::lex() {
  if (_currentLine.isEmpty() && _stream.atEnd()) {
    return {Token::T_END_OF_STREAM, "", _stack.back().line, _stack.back().column };
  } else if (_currentLine.isEmpty()) {
    Token token = {Token::T_NEWLINE, "", _stack.back().line, _stack.back().column };
    _stack.back().offset = _stream.pos();
    _currentLine = _stream.readLine();
    _stack.back().line++;
    _stack.back().column = 1;
    return token;
  }
  foreach (auto pattern, _pattern) {
    if (0 == pattern.first.indexIn(_currentLine)) {
      Token token = {pattern.second, pattern.first.cap(1), _stack.back().line, _stack.back().column};
      _stack.back().offset += pattern.first.matchedLength();
      _stack.back().column += token.value.size();
      _currentLine = _currentLine.mid(pattern.first.matchedLength());
      return token;
    }
  }
  QString err = tr("Lexer error %1,%2: Unexpected char '%3'.").arg(_stack.back().line)
      .arg(_stack.back().column).arg(_currentLine.at(0));
  qDebug() << err;
  return {Token::T_ERROR, err, _stack.back().line, _stack.back().column};
}

void
CSVLexer::push() {
  _stack.push_back(_stack.back());
}

void
CSVLexer::pop() {
  if (_stack.size() < 2)
    return;
  _stack.pop_back();
  _stream.seek(_stack.back().offset);
  _currentLine = QString();
}

/* ********************************************************************************************* *
 * Implementation of CSVHandler
 * ********************************************************************************************* */
CSVHandler::CSVHandler(QObject *parent)
  : QObject(parent)
{
  // pass...
}

CSVHandler::~CSVHandler() {
  // pass...
}

bool
CSVHandler::handleRadioId(qint64 id, qint64 line, qint64 column) {
  Q_UNUSED(id);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleRadioName(const QString &name, qint64 line, qint64 column) {
  Q_UNUSED(name);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleIntroLine1(const QString &text, qint64 line, qint64 column) {
  Q_UNUSED(text);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleIntroLine2(const QString &text, qint64 line, qint64 column) {
  Q_UNUSED(text);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleVoxLevel(uint level, qint64 line, qint64 column) {
  Q_UNUSED(level);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleMicLevel(uint level, qint64 line, qint64 column) {
  Q_UNUSED(level);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleDigitalContact(qint64 idx, const QString &name, DigitalContact::Type type, qint64 id,
                                 bool rxTone, qint64 line, qint64 column)
{
  Q_UNUSED(idx);
  Q_UNUSED(name);
  Q_UNUSED(type);
  Q_UNUSED(id);
  Q_UNUSED(rxTone);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleGroupList(qint64 idx, const QString &name, const QList<qint64> &contacts,
                            qint64 line, qint64 column)
{
  Q_UNUSED(idx);
  Q_UNUSED(name);
  Q_UNUSED(contacts);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleDigitalChannel(
    qint64 idx, const QString &name, double rx, double tx, Channel::Power power, qint64 scan,
    qint64 tot, bool ro, DigitalChannel::Admit admit, qint64 color, DigitalChannel::TimeSlot slot,
    qint64 gl, qint64 contact, qint64 line, qint64 column)
{
  Q_UNUSED(idx);
  Q_UNUSED(name);
  Q_UNUSED(rx);
  Q_UNUSED(tx);
  Q_UNUSED(power);
  Q_UNUSED(scan);
  Q_UNUSED(tot);
  Q_UNUSED(ro);
  Q_UNUSED(admit);
  Q_UNUSED(color);
  Q_UNUSED(slot);
  Q_UNUSED(gl);
  Q_UNUSED(contact);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleAnalogChannel(qint64 idx, const QString &name, double rx, double tx, Channel::Power power, qint64 scan,
    qint64 tot, bool ro, AnalogChannel::Admit admit, qint64 squelch, float rxTone, float txTone,
    AnalogChannel::Bandwidth bw, qint64 line, qint64 column)
{
  Q_UNUSED(idx);
  Q_UNUSED(name);
  Q_UNUSED(rx);
  Q_UNUSED(tx);
  Q_UNUSED(power);
  Q_UNUSED(scan);
  Q_UNUSED(tot);
  Q_UNUSED(ro);
  Q_UNUSED(admit);
  Q_UNUSED(squelch);
  Q_UNUSED(rxTone);
  Q_UNUSED(txTone);
  Q_UNUSED(bw);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleZone(qint64 idx, const QString &name, bool a, const QList<qint64> &channels,
                       qint64 line, qint64 column)
{
  Q_UNUSED(idx);
  Q_UNUSED(name);
  Q_UNUSED(a);
  Q_UNUSED(channels);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}

bool
CSVHandler::handleScanList(qint64 idx, const QString &name, qint64 pch1, qint64 pch2, qint64 txch,
                           const QList<qint64> &channels, qint64 line, qint64 column)
{
  Q_UNUSED(idx);
  Q_UNUSED(name);
  Q_UNUSED(pch1);
  Q_UNUSED(pch2);
  Q_UNUSED(txch);
  Q_UNUSED(channels);
  Q_UNUSED(line);
  Q_UNUSED(column);
  return true;
}


/* ********************************************************************************************* *
 * Implementation of CSVParser
 * ********************************************************************************************* */
CSVParser::CSVParser(CSVHandler *handler, QObject *parent)
  : QObject(parent), _handler(handler)
{
  // pass...
}

bool
CSVParser::parse(QTextStream &stream) {
  if (! stream.seek(0))
    return false;

  CSVLexer lexer(stream);

  for (CSVLexer::Token token=lexer.next(); CSVLexer::Token::T_END_OF_STREAM != token.type; token = lexer.next()) {
    if (CSVLexer::Token::T_NEWLINE == token.type)
      continue;
    if ((CSVLexer::Token::T_KEYWORD == token.type) && ("id" == token.value.toLower())) {
      if (! _parse_radio_id(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("name" == token.value.toLower())) {
      if (! _parse_radio_name(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("introline1" == token.value.toLower())) {
      if (! _parse_introline1(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("introline2" == token.value.toLower())) {
      if (! _parse_introline2(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("voxlevel" == token.value.toLower())) {
      if (! _parse_vox_level(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("miclevel" == token.value.toLower())) {
      if (! _parse_mic_level(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("contact" == token.value.toLower())) {
      if (! _parse_digital_contacts(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("grouplist" == token.value.toLower())) {
      if (! _parse_rx_groups(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("digital" == token.value.toLower())) {
      if (! _parse_digital_channels(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("analog" == token.value.toLower())) {
      if (! _parse_analog_channels(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("zone" == token.value.toLower())) {
      if (! _parse_zones(lexer))
        return false;
    } else if ((CSVLexer::Token::T_KEYWORD == token.type) && ("scanlist" == token.value.toLower())) {
      if (! _parse_scanlists(lexer))
        return false;
    } else if (CSVLexer::Token::T_ERROR == token.type) {
      qDebug() << __func__ << "Parse error:" << token.line << "," << token.column << ":" << token.value;
      return false;
    } else {
      qDebug() << __func__ << "Parse error" << token.line << "," << token.column
               << ": Unexpected token" << token.type << "'" << token.value << "'";
      return false;
    }
  }

  return true;
}

bool
CSVParser::_parse_radio_id(CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  if (CSVLexer::Token::T_COLON != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect ':'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 id = token.value.toInt();
  qint64 line=token.line, column=token.column;

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleRadioId(id, line, column);
}

bool
CSVParser::_parse_radio_name(CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  if (CSVLexer::Token::T_COLON != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect ':'.";
    return false;
  }
  token = lexer.next();
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;
  qint64 line=token.line, column=token.column;

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleRadioName(name, line, column);
}

bool
CSVParser::_parse_introline1(CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  if (CSVLexer::Token::T_COLON != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect ':'.";
    return false;
  }
  token = lexer.next();
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString text = token.value;
  qint64 line=token.line, column=token.column;

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleIntroLine1(text, line, column);
}

bool
CSVParser::_parse_introline2(CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  if (CSVLexer::Token::T_COLON != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect ':'.";
    return false;
  }
  token = lexer.next();
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString text = token.value;
  qint64 line=token.line, column=token.column;

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleIntroLine2(text, line, column);
}

bool
CSVParser::_parse_vox_level(CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  if (CSVLexer::Token::T_COLON != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect ':'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 level = token.value.toInt();
  qint64 line=token.line, column=token.column;

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleVoxLevel(level, line, column);
}

bool
CSVParser::_parse_mic_level(CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  if (CSVLexer::Token::T_COLON != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect ':'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 level = token.value.toInt();
  qint64 line=token.line, column=token.column;

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleMicLevel(level, line, column);
}

bool
CSVParser::_parse_digital_contacts(CSVLexer &lexer) {
  // skip rest of header
  CSVLexer::Token token = lexer.next();
  for (; CSVLexer::Token::T_KEYWORD==token.type; token=lexer.next()) {
    // skip
  }
  if (CSVLexer::Token::T_NEWLINE != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
    return false;
  }

  token = lexer.next();
  for (; CSVLexer::Token::T_NUMBER == token.type; token=lexer.next()) {
    qint64 idx = token.value.toInt();
    if (! _parse_digital_contact(idx, lexer))
      return false;
  }

  if ((CSVLexer::Token::T_NEWLINE == token.type) || (CSVLexer::Token::T_END_OF_STREAM == token.type))
    return true;

  qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
           << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
  return false;
}

bool
CSVParser::_parse_digital_contact(qint64 idx, CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  qint64 line=token.line, column=token.column;

  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;

  token = lexer.next();
  if (CSVLexer::Token::T_KEYWORD != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  DigitalContact::Type type;
  if ("group" == token.value.toLower()) {
    type = DigitalContact::GroupCall;
  } else if ("private" == token.value.toLower()) {
    type = DigitalContact::PrivateCall;
  } else if ("all" == token.value.toLower()) {
    type = DigitalContact::AllCall;
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected call type '" << token.value << "', expect 'Group', 'Private' or 'All'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 id = token.value.toInt();

  bool rxToneEnabled;
  token = lexer.next();
  if (CSVLexer::Token::T_NOT_SET == token.type)
    rxToneEnabled = false;
  else if (CSVLexer::Token::T_ENABLED == token.type)
    rxToneEnabled = true;
  else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect '-' or '+'.";
    return false;
  }

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleDigitalContact(idx, name, type, id, rxToneEnabled, line, column);
}


bool
CSVParser::_parse_rx_groups(CSVLexer &lexer) {
  // skip rest of header
  CSVLexer::Token token = lexer.next();
  for (; CSVLexer::Token::T_KEYWORD==token.type; token=lexer.next()) {
    // skip
  }
  if (CSVLexer::Token::T_NEWLINE != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
    return false;
  }

  token = lexer.next();
  for (; CSVLexer::Token::T_NUMBER == token.type; token=lexer.next()) {
    qint64 idx = token.value.toInt();
    if (! _parse_rx_group(idx, lexer))
      return false;
  }

  if ((CSVLexer::Token::T_NEWLINE == token.type) || (CSVLexer::Token::T_END_OF_STREAM == token.type))
    return true;

  qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
           << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";

  return false;
}

bool
CSVParser::_parse_rx_group(qint64 idx, CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  qint64 line=token.line, column=token.column;
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;

  QList<qint64> lst;
  token = lexer.next();
  while (CSVLexer::Token::T_NUMBER == token.type) {
    lst.append(token.value.toInt());
    token = lexer.next();
    if (CSVLexer::Token::T_COMMA == token.type)
      token = lexer.next();
  }

  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleGroupList(idx, name, lst, line, column);
}

bool
CSVParser::_parse_digital_channels(CSVLexer &lexer) {
  // skip rest of header
  CSVLexer::Token token = lexer.next();
  for (; CSVLexer::Token::T_KEYWORD==token.type; token=lexer.next()) {
    // skip
  }
  if (CSVLexer::Token::T_NEWLINE != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
    return false;
  }

  token = lexer.next();
  for (; CSVLexer::Token::T_NUMBER == token.type; token=lexer.next()) {
    qint64 idx = token.value.toInt();
    if (! _parse_digital_channel(idx, lexer))
      return false;
  }

  if ((CSVLexer::Token::T_NEWLINE == token.type) || (CSVLexer::Token::T_END_OF_STREAM == token.type))
    return true;

  qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
           << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
  return false;
}

bool
CSVParser::_parse_digital_channel(qint64 idx, CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  qint64 line=token.line, column=token.column;
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  double rx = token.value.toDouble();

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  double tx = token.value.toDouble();
  if (token.value.startsWith('+') || token.value.startsWith('-'))
    tx = rx + tx;

  token = lexer.next();
  if (CSVLexer::Token::T_KEYWORD != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect keyword.";
    return false;
  }
  Channel::Power pwr;
  if ("high" == token.value.toLower())
    pwr = Channel::HighPower;
  else if ("low" == token.value.toLower())
    pwr = Channel::LowPower;
  else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected keyword '" << token.value << "', expect 'High' or 'Low'.";
    return false;
  }

  token = lexer.next();
  qint64 scanlist;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    scanlist = 0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    scanlist = token.value.toInt();
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number or '-'.";
    return false;
  }

  token = lexer.next();
  if ((CSVLexer::Token::T_NUMBER != token.type) && (CSVLexer::Token::T_NOT_SET != token.type)) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 tot =  (CSVLexer::Token::T_NOT_SET != token.type) ? 0 : token.value.toInt();

  bool rxOnly;
  token = lexer.next();
  if (CSVLexer::Token::T_NOT_SET == token.type)
    rxOnly = false;
  else if (CSVLexer::Token::T_ENABLED == token.type)
    rxOnly = true;
  else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect '-' or '+'.";
    return false;
  }

  token = lexer.next();
  DigitalChannel::Admit admit;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    admit = DigitalChannel::AdmitNone;
  } else if (CSVLexer::Token::T_KEYWORD == token.type) {
    if ("free" == token.value.toLower())
      admit = DigitalChannel::AdmitFree;
    else if ("color" == token.value.toLower())
      admit = DigitalChannel::AdmitColorCode;
    else {
      qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
               << ": Unexpected keyword '" << token.value << "', expect 'Free' or 'Color'.";
      return false;
    }
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect 'Free', 'Color' or '-'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 color = token.value.toInt();

  token = lexer.next();
  DigitalChannel::TimeSlot slot;
  if (CSVLexer::Token::T_NUMBER == token.type) {
    if (1 == token.value.toInt()) {
      slot = DigitalChannel::TimeSlot1;
    } else if (2 == token.value.toInt()) {
      slot = DigitalChannel::TimeSlot2;
    } else {
      qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
               << ": Unexpected value '" << token.value << "', expect '1' or '2'.";
      return false;
    }
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }

  token = lexer.next();
  qint64 rxGroupList;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    rxGroupList = 0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    rxGroupList = token.value.toInt();
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number or '-'.";
    return false;
  }

  token = lexer.next();
  qint64 txContact;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    txContact = 0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    txContact = token.value.toInt();
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number or '-'.";
    return false;
  }

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleDigitalChannel(idx, name, rx, tx, pwr, scanlist, tot, rxOnly, admit, color,
                                        slot, rxGroupList, txContact, line, column);
}

bool
CSVParser::_parse_analog_channels(CSVLexer &lexer) {
  // skip rest of header
  CSVLexer::Token token = lexer.next();
  for (; CSVLexer::Token::T_KEYWORD==token.type; token=lexer.next()) {
    // skip
  }
  if (CSVLexer::Token::T_NEWLINE != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
    return false;
  }

  token = lexer.next();
  for (; CSVLexer::Token::T_NUMBER == token.type; token=lexer.next()) {
    qint64 idx = token.value.toInt();
    if (! _parse_analog_channel(idx, lexer))
      return false;
  }

  if ((CSVLexer::Token::T_NEWLINE == token.type) || (CSVLexer::Token::T_END_OF_STREAM == token.type))
    return true;

  qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
           << ": Unexpected token" << token.type << "'" << token.value << "', expect newline or EOS.";
  return false;
}

bool
CSVParser::_parse_analog_channel(qint64 idx, CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  qint64 line=token.line, column=token.column;
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  double rx = token.value.toDouble();

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  double tx = token.value.toDouble();
  if (token.value.startsWith('+') || token.value.startsWith('-'))
    tx = rx + tx;

  token = lexer.next();
  if (CSVLexer::Token::T_KEYWORD != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect keyword.";
    return false;
  }
  Channel::Power pwr;
  if ("high" == token.value.toLower())
    pwr = Channel::HighPower;
  else if ("low" == token.value.toLower())
    pwr = Channel::LowPower;
  else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected keyword '" << token.value << "', expect 'High' or 'Low'.";
    return false;
  }

  token = lexer.next();
  qint64 scanlist;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    scanlist = 0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    scanlist = token.value.toInt();
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number or '-'.";
    return false;
  }

  token = lexer.next();
  if ((CSVLexer::Token::T_NUMBER != token.type) && (CSVLexer::Token::T_NOT_SET != token.type)) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 tot =  (CSVLexer::Token::T_NOT_SET != token.type) ? 0 : token.value.toInt();

  bool rxOnly;
  token = lexer.next();
  if (CSVLexer::Token::T_NOT_SET == token.type)
    rxOnly = false;
  else if (CSVLexer::Token::T_ENABLED == token.type)
    rxOnly = true;
  else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect '-' or '+'.";
    return false;
  }

  token = lexer.next();
  AnalogChannel::Admit admit;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    admit = AnalogChannel::AdmitNone;
  } else if (CSVLexer::Token::T_KEYWORD == token.type) {
    if ("free" == token.value.toLower())
      admit = AnalogChannel::AdmitFree;
    else if ("color" == token.value.toLower())
      admit = AnalogChannel::AdmitTone;
    else {
      qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
               << ": Unexpected keyword '" << token.value << "', expect 'Free' or 'Tone'.";
      return false;
    }
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect 'Free', 'Tone' or '-'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  qint64 squelch = token.value.toInt();

  token = lexer.next();
  float rxTone;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    rxTone = 0.0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    rxTone = token.value.toFloat();
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number or '-'.";
    return false;
  }

  token = lexer.next();
  float txTone;
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    txTone = 0.0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    txTone = token.value.toFloat();
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number or '-'.";
    return false;
  }

  token = lexer.next();
  if (CSVLexer::Token::T_NUMBER != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect number.";
    return false;
  }
  AnalogChannel::Bandwidth bw;
  if (25 == token.value.toFloat()) {
    bw = AnalogChannel::BWWide;
  } else if (12.5 == token.value.toFloat()) {
    bw = AnalogChannel::BWNarrow;
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected bandwidth value '" << token.value << "', expect '12.5' or '25'.";
    return false;
  }

  token = lexer.next();
  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleAnalogChannel(idx, name, rx, tx, pwr, scanlist, tot, rxOnly, admit, squelch,
                                       rxTone, txTone, bw, line, column);
}

bool
CSVParser::_parse_zones(CSVLexer &lexer) {
  // skip rest of header
  CSVLexer::Token token = lexer.next();
  for (; CSVLexer::Token::T_KEYWORD==token.type; token=lexer.next()) {
    // skip
  }
  if (CSVLexer::Token::T_NEWLINE != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
    return false;
  }

  token = lexer.next();
  for (; CSVLexer::Token::T_NUMBER == token.type; token=lexer.next()) {
    qint64 idx = token.value.toInt();
    if (! _parse_zone(idx, lexer))
      return false;
  }

  if ((CSVLexer::Token::T_NEWLINE == token.type) || (CSVLexer::Token::T_END_OF_STREAM == token.type))
    return true;

  qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
           << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
  return false;
}

bool
CSVParser::_parse_zone(qint64 idx, CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  qint64 line=token.line, column=token.column;
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;

  token = lexer.next();
  if ((CSVLexer::Token::T_KEYWORD != token.type) || (("a" != token.value.toLower()) && ("b" != token.value.toLower())) ) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect 'A' or 'B'.";
    return false;
  }
  bool a = ("a" == token.value.toLower());

  QList<qint64> lst;
  token = lexer.next();
  while (CSVLexer::Token::T_NUMBER == token.type) {
    lst.append(token.value.toInt());
    token = lexer.next();
    if (CSVLexer::Token::T_COMMA == token.type)
      token = lexer.next();
  }

  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleZone(idx, name, a, lst, line, column);
}

bool
CSVParser::_parse_scanlists(CSVLexer &lexer) {
  // skip rest of header
  CSVLexer::Token token = lexer.next();
  for (; CSVLexer::Token::T_KEYWORD==token.type; token=lexer.next()) {
    // skip
  }
  if (CSVLexer::Token::T_NEWLINE != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
    return false;
  }

  token = lexer.next();
  for (; CSVLexer::Token::T_NUMBER == token.type; token=lexer.next()) {
    qint64 idx = token.value.toInt();
    if (! _parse_scanlist(idx, lexer))
      return false;
  }

  if ((CSVLexer::Token::T_NEWLINE == token.type) || (CSVLexer::Token::T_END_OF_STREAM == token.type))
    return true;

  qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
           << ": Unexpected token" << token.type << "'" << token.value << "', expect newline.";
  return false;
}

bool
CSVParser::_parse_scanlist(qint64 idx, CSVLexer &lexer) {
  CSVLexer::Token token = lexer.next();
  qint64 line=token.line, column=token.column;
  if (CSVLexer::Token::T_STRING != token.type) {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect string.";
    return false;
  }
  QString name = token.value;

  qint64 pch1;
  token = lexer.next();
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    pch1 = 0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    pch1 = token.value.toInt() + 1;
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect '-' or number.";
    return false;
  }

  qint64 pch2;
  token = lexer.next();
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    pch2 = 0;
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    pch2 = token.value.toInt() + 1;
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect '-' or number.";
    return false;
  }

  qint64 txch;
  token = lexer.next();
  if (CSVLexer::Token::T_NOT_SET == token.type) {
    txch = 0;
  } else if (CSVLexer::Token::T_KEYWORD == token.type) {
    if ("sel" == token.value.toLower()) {
      txch = 1;
    } else {
      qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
               << ": Unexpected token" << token.type << "'" << token.value << "', expect '-', 'Sel' or number.";
      return false;
    }
  } else if (CSVLexer::Token::T_NUMBER == token.type) {
    txch = token.value.toInt() + 1;
  } else {
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect '-', 'Sel' or number.";
    return false;
  }

  QList<qint64> lst;
  token = lexer.next();
  while (CSVLexer::Token::T_NUMBER == token.type) {
    lst.append(token.value.toInt());
    token = lexer.next();
    if (CSVLexer::Token::T_COMMA == token.type)
      token = lexer.next();
  }

  if ((CSVLexer::Token::T_NEWLINE != token.type) && (CSVLexer::Token::T_END_OF_STREAM != token.type)){
    qDebug() << __func__ << "Parse error @" << token.line << "," << token.column
             << ": Unexpected token" << token.type << "'" << token.value << "', expect newline/EOS.";
    return false;
  }

  return _handler->handleScanList(idx, name, pch1, pch2, txch, lst, line, column);
}


/* ********************************************************************************************* *
 * Implementation of CSVReader
 * ********************************************************************************************* */
CSVReader::CSVReader(Config *config, QObject *parent)
  : CSVHandler(parent), _link(false), _config(config)
{
  // pass...
}


bool
CSVReader::read(Config *config, QTextStream &stream) {
  // Seek to start-of-stream
  if (! stream.seek(0)) {
    qDebug() << __func__ << "Cannot read CSV codeplug: Cannot seek within text stream. Abort import." ;
    return false;
  }

  config->reset();

  CSVReader reader(config);
  CSVParser parser(&reader);

  if (! parser.parse(stream))
    return false;

  reader._link = true;

  if (! parser.parse(stream))
    return false;

  return true;
}


bool
CSVReader::handleRadioId(qint64 id, qint64 line, qint64 column) {
  Q_UNUSED(line);
  Q_UNUSED(column);

  if (_link) {
    _config->setId(id);
  }
  return true;
}

bool
CSVReader::handleRadioName(const QString &name, qint64 line, qint64 column) {
  Q_UNUSED(line);
  Q_UNUSED(column);

  if (_link) {
    _config->setName(name);
  }
  return true;
}

bool
CSVReader::handleIntroLine1(const QString &text, qint64 line, qint64 column) {
  Q_UNUSED(line);
  Q_UNUSED(column);

  if (_link) {
    _config->setIntroLine1(text);
  }
  return true;
}

bool
CSVReader::handleIntroLine2(const QString &text, qint64 line, qint64 column) {
  Q_UNUSED(line);
  Q_UNUSED(column);

  if (_link) {
    _config->setIntroLine2(text);
  }
  return true;
}

bool
CSVReader::handleVoxLevel(uint level, qint64 line, qint64 column) {
  Q_UNUSED(line);
  Q_UNUSED(column);

  if (_link) {
    _config->setVoxLevel(level);
  }
  return true;
}

bool
CSVReader::handleMicLevel(uint level, qint64 line, qint64 column) {
  Q_UNUSED(line);
  Q_UNUSED(column);

  if (_link) {
    _config->setMicLevel(level);
  }
  return true;
}

bool
CSVReader::handleDigitalContact(qint64 idx, const QString &name, DigitalContact::Type type,
                                qint64 id, bool rxTone, qint64 line, qint64 column)
{
  if (_link) {
    // pass...
    return true;
  }

  if (_contacts.contains(idx)) {
    qDebug() << "Parse error @" << line << "," << column
             << ": Cannot create contact '" << name << "' with index " << idx
             << " index already taken.";
    return false;
  }
  DigitalContact *contact = new DigitalContact(type, name, id, rxTone);
  _contacts[idx] = contact;
  _config->contacts()->addContact(contact);

  return true;
}

bool
CSVReader::handleGroupList(qint64 idx, const QString &name, const QList<qint64> &contacts,
                           qint64 line, qint64 column)
{
  if (_link) {
    foreach(qint64 i, contacts) {
      // Check contacts
      if (! _contacts.contains(i)) {
        qDebug() << "Parse error @" << line << "," << column
                 << ": Cannot create contact list '" << name
                 << ": Unknwon contact index " << i;
        return false;
      }
      // link contacts
      _rxgroups[idx]->addContact(_contacts[i]);
    }
    // done
    return true;
  }

  // check index
  if (_rxgroups.contains(idx)) {
    qDebug() << "Parse error @" << line << "," << column
             << ": Cannot create RX group-list '" << name << "' with index " << idx
             << " index already taken.";
    return false;
  }
  // Create group list
  RXGroupList *lst = new RXGroupList(name);
  _config->rxGroupLists()->addList(lst);
  _rxgroups[idx] = lst;

  return true;
}

bool
CSVReader::handleDigitalChannel(
    qint64 idx, const QString &name, double rx, double tx, Channel::Power power, qint64 scan,
    qint64 tot, bool ro, DigitalChannel::Admit admit, qint64 color, DigitalChannel::TimeSlot slot,
    qint64 gl, qint64 contact, qint64 line, qint64 column)
{
  if (_link) {
    // Check RX Grouplist
    if ((0 < gl) && (! _rxgroups.contains(gl))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot link digital channel '" << name
               << ": Unknwon RX group-list-index " << gl;
      return false;
    }
    _channels[idx]->as<DigitalChannel>()->setRXGroupList(_rxgroups[gl]);
    // Check TX Contact
    if ((0 < contact) && (! _contacts.contains(contact))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot link digital channel '" << name
               << ": Unknwon contact-index " << contact;
      return false;
    }
    _channels[idx]->as<DigitalChannel>()->setTXContact(_contacts[contact]);
    // Check scanlist
    if ((0 < scan) && (! _scanlists.contains(scan))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot link digital channel '" << name
               << ": Unknwon scanlist-index " << scan;
      return false;
    }
    _channels[idx]->as<DigitalChannel>()->setScanList(_scanlists[scan]);
    return true;
  }

  // check index
  if (_channels.contains(idx)) {
    qDebug() << "Parse error @" << line << "," << column
             << ": Cannot create digital channel '" << name << "' with index " << idx
             << " index already taken.";
    return false;
  }

  DigitalChannel *chan = new DigitalChannel(name, rx, tx, power, tot, ro, admit, color, slot,
                                            nullptr, nullptr, nullptr);
  _config->channelList()->addChannel(chan);
  _channels[idx] = chan;

  return true;
}


bool
CSVReader::handleAnalogChannel(
    qint64 idx, const QString &name, double rx, double tx, Channel::Power power, qint64 scan,
    qint64 tot, bool ro, AnalogChannel::Admit admit, qint64 squelch, float rxTone, float txTone,
    AnalogChannel::Bandwidth bw, qint64 line, qint64 column)
{
  if (_link) {
    // Check scanlist
    if ((0 < scan) && (! _scanlists.contains(scan))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot link analog channel '" << name
               << ": Unknwon scanlist-index " << scan;
      return false;
    }
    _channels[idx]->as<AnalogChannel>()->setScanList(_scanlists[scan]);
    return true;
  }

  // check index
  if (_channels.contains(idx)) {
    qDebug() << "Parse error @" << line << "," << column
             << ": Cannot create analog channel '" << name << "' with index " << idx
             << " index already taken.";
    return false;
  }

  AnalogChannel *chan = new AnalogChannel(name, rx, tx, power, tot, ro, admit, squelch, rxTone,
                                          txTone, bw, nullptr);
  _config->channelList()->addChannel(chan);
  _channels[idx] = chan;

  return true;
}

bool
CSVReader::handleZone(qint64 idx, const QString &name, bool a, const QList<qint64> &channels, qint64 line, qint64 column)
{
  if (_link) {
    foreach(qint64 i, channels) {
      // Check channels
      if (! _channels.contains(i)) {
        qDebug() << "Parse error @" << line << "," << column
                 << ": Cannot create zone '" << name
                 << ": Unknwon channel index " << i;
        return false;
      }
      // link channels
      if (a)
        _zones[idx]->A()->addChannel(_channels[i]);
      else
        _zones[idx]->B()->addChannel(_channels[i]);
    }
    // done
    return true;
  }

  // check index
  if (! _zones.contains(idx)) {
    Zone *zone = new Zone(name);
    _zones[idx] = zone;
    _config->zones()->addZone(zone);
  }

  return true;
}

bool
CSVReader::handleScanList(qint64 idx, const QString &name, qint64 pch1, qint64 pch2, qint64 txch,
                          const QList<qint64> &channels, qint64 line, qint64 column)
{
  if (_link) {
    // Check PriChan 1
    if (pch1 && (! _channels.contains(pch1))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot create scanlist '" << name
               << ": Unknwon priority channel 1 index " << pch1;
      return false;
    }
    _scanlists[idx]->setPriorityChannel(_channels[pch1]);

    // Check PriChan 2
    if (pch2 && (! _channels.contains(pch2))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot create scanlist '" << name
               << ": Unknwon priority channel 2 index " << pch2;
      return false;
    }
    _scanlists[idx]->setSecPriorityChannel(_channels[pch2]);

    // Check Tx channel
    if ((txch>1) && (! _channels.contains(txch-1))) {
      qDebug() << "Parse error @" << line << "," << column
               << ": Cannot create scanlist '" << name
               << ": Unknwon priority channel 2 index " << txch-1;
      return false;
    }
    /// @todo set TX Channel

    // Check channels
    foreach(qint64 i, channels) {
      if (! _channels.contains(i)) {
        qDebug() << "Parse error @" << line << "," << column
                 << ": Cannot create scanlist '" << name
                 << ": Unknwon channel index " << i;
        return false;
      }
      // link channels
      _scanlists[idx]->addChannel(_channels[i]);
    }
    // done
    return true;
  }

  // check index
  if (_scanlists.contains(idx)) {
    qDebug() << "Parse error @" << line << "," << column
             << ": Cannot create zone '" << name << "' with index " << idx
             << " index already taken.";
    return false;
  }

  ScanList *lst = new ScanList(name);
  _scanlists[idx] = lst;
  _config->scanlists()->addScanList(lst);

  return true;
}
