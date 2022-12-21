#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <di.hpp>
#include "worldmap.h"
#include "Abilities.h"
#include "abilityimpl.h"

namespace di = boost::di;

int main(int argc, char *argv[])
{

    using mWorldMap = WorldMap<256,128>;

    const auto injector = di::make_injector(
        di::bind<mWorldMap>.to(std::make_shared<mWorldMap>()),
        di::bind<ISensor>.to<SensorImpl>(),
        di::bind<IMotion>.to<MotionImpl>(),
        di::bind<IDivider>.to<DividerImlp>()
        );

    auto wm = injector.create<mWorldMap>();

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:/creatureA/main.qml"_qs);
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
        &app, [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
