#include "argumentsconfigure.h"

#include <QStringList>

ArgumentsConfigure::ArgumentsConfigure()
{
    parse();
}

void ArgumentsConfigure::parse()
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

