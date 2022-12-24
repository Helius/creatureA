#include "../WorldGeometry.h"
#include <QtTest/QtTest>

class TestWorldGeometry : public QObject {

    Q_OBJECT

private slots:
    void TestDirectionRotation();
    void TestDirectionOffset();
    void TestGeometryIndexOutOfRange();
    void TestGeometryStartInd();
    void TestGeometryCengerInd();
    void TestGeometryEnd();
    void TestGeometryBottom();
};


void TestWorldGeometry::TestDirectionRotation()
{
    Direction dir1;
    Direction dir2;
    QCOMPARE(dir1, dir2);
    dir1.turnRight();
    QVERIFY(!(dir1 == dir2));
    dir1.turnLeft();
    QCOMPARE(dir1, dir2);
    for (uint i = 0; i < Direction::maxNumber; ++i) {
        dir1.turnLeft();
    }
    QCOMPARE(dir1, dir2);
}

void TestWorldGeometry::TestDirectionOffset()
{
    Direction dir; // up
    QCOMPARE(dir.offsetX(), 0);
    QCOMPARE(dir.offsetY(), -1);
    dir.turnRight(); // up + right
    QCOMPARE(dir.offsetX(), 1);
    QCOMPARE(dir.offsetY(), -1);
    dir.turnRight(); // right
    QCOMPARE(dir.offsetX(), 1);
    QCOMPARE(dir.offsetY(), 0);
    dir.turnRight(); // down + right
    QCOMPARE(dir.offsetX(), 1);
    QCOMPARE(dir.offsetY(), 1);
    dir.turnRight(); // down
    QCOMPARE(dir.offsetX(), 0);
    QCOMPARE(dir.offsetY(), 1);

    dir.turnRight(); // down + left
    QCOMPARE(dir.offsetX(), -1);
    QCOMPARE(dir.offsetY(), 1);
    dir.turnRight(); // left
    QCOMPARE(dir.offsetX(), -1);
    QCOMPARE(dir.offsetY(), 0);
    dir.turnRight(); // left + up
    QCOMPARE(dir.offsetX(), -1);
    QCOMPARE(dir.offsetY(), -1);
    dir.turnRight(); // up
    QCOMPARE(dir.offsetX(), 0);
    QCOMPARE(dir.offsetY(), -1);
}

void TestWorldGeometry::TestGeometryIndexOutOfRange()
{
    NearestSpace ns(100, 3, 4);
    Direction dir;
    QVERIFY(!ns.go(dir));
}

void TestWorldGeometry::TestGeometryStartInd()
{
    // 012
    // 345

    NearestSpace ns(0, 3, 2);
    Direction dir; // up
    QVERIFY(!ns.go(dir));

    dir.turnRight(); // up + right
    QVERIFY(!ns.go(dir));

    dir.turnRight(); // right
    auto iopt = ns.go(dir);
    QVERIFY(iopt);
    QCOMPARE(*iopt, 1);

    dir.turnRight(); //  down + right
    iopt = ns.go(dir);
    QVERIFY(iopt);
    QCOMPARE(*iopt, 4);

    dir.turnRight(); //  down
    iopt = ns.go(dir);
    QVERIFY(iopt);
    QCOMPARE(*iopt, 3);

    dir.turnRight(); //  down + left
    iopt = ns.go(dir);
    QVERIFY(!iopt);

    dir.turnRight(); //  down + right
    iopt = ns.go(dir);
    QVERIFY(!iopt);
}

void TestWorldGeometry::TestGeometryCengerInd()
{
    // 0   1   2   3    4   ...
    // 256 257 258 259 [260] 261 ...
    // 512 513 514 515 516 ...

    Direction dir; // up
    NearestSpace ns(260, 256, 128);
    QCOMPARE(*ns.go(dir), 4);
    dir.turnRight(); // up+right
    QCOMPARE(*ns.go(dir), 5);
    dir.turnRight(); // right
    QCOMPARE(*ns.go(dir), 261);
    dir.turnRight(); // right + down
    QCOMPARE(*ns.go(dir), 517);
    dir.turnRight(); // down
    QCOMPARE(*ns.go(dir), 516);
    dir.turnRight(); // down + left
    QCOMPARE(*ns.go(dir), 515);
    dir.turnRight(); // left
    QCOMPARE(*ns.go(dir), 259);
    dir.turnRight(); // left + up
    QCOMPARE(*ns.go(dir), 3);
}

void TestWorldGeometry::TestGeometryEnd()
{
    // 0,1,2, 3
    // 4,5,6, 7
    // 8,9,10,11
    Direction dir;
    NearestSpace ns(11,4,3);
    QCOMPARE(*ns.go(dir), 7);
    dir.turnRight(); // up+right
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // right
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // right + down
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // down
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // down + left
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // left
    QCOMPARE(*ns.go(dir), 10);
    dir.turnRight(); // left + up
    QCOMPARE(*ns.go(dir), 6);

}

void TestWorldGeometry::TestGeometryBottom()
{
    // 0,1,2, 3
    // 4,5,6, 7
    // 8,9,10,11
    Direction dir;
    NearestSpace ns(10,4,3);
    QCOMPARE(*ns.go(dir), 6);
    dir.turnRight(); // up+right
    QCOMPARE(ns.go(dir), 7);
    dir.turnRight(); // right
    QCOMPARE(ns.go(dir), 11);
    dir.turnRight(); // right + down
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // down
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // down + left
    QCOMPARE(!!ns.go(dir), false);
    dir.turnRight(); // left
    QCOMPARE(*ns.go(dir), 9);
    dir.turnRight(); // left + up
    QCOMPARE(*ns.go(dir), 5);
}

QTEST_MAIN(TestWorldGeometry)

#include "test_WorldGeometry.moc"