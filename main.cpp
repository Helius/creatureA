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
#include "Abilities.h"
#include "abilityimpl.h"
#include "worldprocessor.h"
#include "CreatureBuilderImpl.h"
#include "ICreatureBuilder.h"


namespace di = boost::di;

int main(int argc, char *argv[])
{

    const auto injector = di::make_injector(
        di::bind<WorldMap>.to(std::make_shared<WorldMap>(256, 128)),
        di::bind<WorldProcessor>.to<WorldProcessor>(),
        di::bind<ISensor>.to<SensorImpl>(),
        di::bind<IMotion>.to<MotionImpl>(),
        di::bind<IDivider>.to<CreatureCreatorImlp>(),
        di::bind<ICreatureBuilder>.to<CreatureBuilderImpl>(),
        di::bind<WorldMapModel>.to<WorldMapModel>()
        );

    QmlInjectorBuilder builder;
    builder.add<WorldMapModel>([&injector](const QVariant&) -> WorldMapModelUnq {
        return injector.create<WorldMapModelUnq>();
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
