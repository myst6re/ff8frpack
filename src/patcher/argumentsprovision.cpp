#include "argumentsprovision.h"

#include <QStringList>

ArgumentsProvision::ArgumentsProvision()
{
    parse();
}

void ArgumentsProvision::parse()
{
    _parser.process(*qApp);

    QStringList args = _parser.positionalArguments();

    if (args.isEmpty() || args.size() != 2) {
        qWarning() << qPrintable("Wrong number of arguments");
        showHelp();
    }

    _ff8Directory = args.first();
    _ff8frPackDirectory = args.at(1);
}
