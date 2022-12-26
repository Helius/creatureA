#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include <lib/di.hpp>

#include <lib/qmlinjector/iqmlobjectcreator.h>
#include <lib/qmlinjector/qmlinjector.h>
#include <lib/qmlinjector/qmlinjectorbuilder.h>

#include <presentation/WorldMapModel.h>

#include "worldmap.h"
#include "worldprocessor.h"
#include "presentation/WorldControllerPresenter.h"


namespace di = boost::di;

int main(int argc, char *argv[])
{

    const auto injector = di::make_injector(
        di::bind<WorldMap>.to(std::make_shared<WorldMap>(450, 200)),
        di::bind<WorldProcessor>.to<WorldProcessor>(),
        di::bind<WorldMapModel>.to<WorldMapModel>(),
        di::bind<IWorldController>.to<WorldProcessor>()
        );

    QmlInjectorBuilder builder;
    builder.add<WorldMapModel>([&injector](const QVariant&) -> WorldMapModelUnq {
        return injector.create<WorldMapModelUnq>();
    });
    builder.add<WorldControllerPresenter>([&injector](const QVariant&) -> WorldControllerPresenterUnq {
        return injector.create<WorldControllerPresenterUnq>();
    });

    qmlRegisterType<QmlInjector>("injector", 1, 0, "QmlInjector");

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextObject(builder.build());
    const QUrl url(u"qrc:/creatureA/qml/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
