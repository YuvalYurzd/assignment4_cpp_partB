#include "doctest.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include "sources/Team.hpp"
#include "sources/Team2.hpp"
#include "sources/SmartTeam.hpp"
#include <float.h>

TEST_CASE("Testing Point Class")
{
    SUBCASE("Constructor")
    {
        CHECK_NOTHROW(Point p2(1.1, 2.2));
        CHECK_NOTHROW(Point(0, -0));
        double x = 3.14;
        double y = -2.71;
        CHECK_NOTHROW(Point p4(x, y));
        Point p1(1.0, 2.0);
        CHECK(p1.getX() == 1.0);
        CHECK(p1.getY() == 2.0);
    }

    SUBCASE("Functions dont throw exceptions")
    {
        Point p1(1, 2);
        Point p2(4, 3);
        Point p3(-1, -7);
        Point p4(-5, 0);

        CHECK_NOTHROW(p1.distance(p2));
        CHECK_NOTHROW(p3.distance(p4));
        CHECK_NOTHROW(p1.distance(p1));
        CHECK_NOTHROW(p2.moveTowards(p2, p4, 2));
        CHECK_NOTHROW(p1.moveTowards(p1, p3, -8));
        CHECK_NOTHROW(p1.moveTowards(p1, p2, 0));
        CHECK_NOTHROW(p1.print());
        CHECK_NOTHROW(p2.print());
    }

    SUBCASE("Distance method")
    {
        Point p1(0.0, 0.0);
        Point p2(3.0, 4.0);
        Point p3(5, 12);
        Point p4(8, 15);
        Point p5(0, 0);
        Point p6(6, 8);
        CHECK(p1.distance(p2) == 5.0);
        CHECK(p1.distance(p3) == 13);
        CHECK(p1.distance(p4) == 17);
        CHECK(p3.distance(p4) > 0);
        CHECK(p4.distance(p3) > 0);
        CHECK(p1.distance(p5) == 0);
        CHECK_EQ(p1.distance(p2), p2.distance(p6));
        CHECK_EQ(p1.distance(p2), p2.distance(p1));
        CHECK_EQ(p1.distance(p1), 0);
    }

    SUBCASE("Distance method - Points on the same axis")
    {
        Point p1(5, 0);
        Point p2(5, 6);
        Point p3(0, 3);
        Point p4(8, 3);

        CHECK(p1.distance(p2) == 6.0);
        CHECK(p3.distance(p4) == 8.0);
    }

    SUBCASE("Move Towards method - both directions (p1 -> p2) && (p2 -> p1)")
    {
        Point p1(0.0, 0.0);
        Point p2(3.0, 4.0);

        SUBCASE("Move full distance")
        {
            Point result1 = p1.moveTowards(p1, p2, 5.0);
            CHECK(result1.getX() == 3.0);
            CHECK(result1.getY() == 4.0);

            Point result2 = p2.moveTowards(p2, p1, 5.0);
            CHECK(result2.getX() == 0);
            CHECK(result2.getY() == 0);
        }

        SUBCASE("Move partial distance")
        {
            Point result1 = p1.moveTowards(p1, p2, 2.5);
            CHECK(result1.distance(p2) > 0);
            CHECK(result1.distance(p1) > 0);

            Point result2 = p2.moveTowards(p2, p1, 2.5);
            CHECK(result2.distance(p2) > 0);
            CHECK(result2.distance(p1) > 0);

            CHECK_EQ(result1.getX(), 1.5);
            CHECK_EQ(result2.getX(), 1.5);
            CHECK_EQ(result1.getY(), 2);
            CHECK_EQ(result2.getY(), 2);
        }

        SUBCASE("Move zero distance")
        {
            Point result1 = p1.moveTowards(p1, p2, 0.0);
            CHECK_EQ(result1.getX(), p1.getX());
            CHECK_EQ(result1.getY(), p1.getY());

            Point result2 = p2.moveTowards(p2, p1, 0.0);
            CHECK_EQ(result2.getX(), p2.getX());
            CHECK_EQ(result2.getY(), p2.getY());
        }

        SUBCASE("Negative distance")
        {
            CHECK_THROWS_AS(Point result1 = p1.moveTowards(p1, p2, -5.0),std::invalid_argument);
            CHECK_THROWS_AS(Point result2 = p2.moveTowards(p2, p1, -2.0),std::invalid_argument);
        }

        SUBCASE("Source and destination points are the same")
        {
            Point p1(3, 4);
            Point result = Point::moveTowards(p1, p1, 5.0);

            CHECK(result.getX() == 3.0);
            CHECK(result.getY() == 4.0);
        }
    }

    SUBCASE("Large numbers")
    {
        Point p1(1e308, 1e308);
        Point p2(1e308, 1e308);
        Point p3(1e308 - 1000, 1e308 - 1000);

        // Test getX() and getY() with large numbers
        CHECK(p1.getX() == 1e308);
        CHECK(p1.getY() == 1e308);

        // Test distance() with large numbers
        double distance_p1_p2 = p1.distance(p2);
        CHECK(distance_p1_p2 == doctest::Approx(0));

        // Test moveTowards() with large numbers
        Point result = Point::moveTowards(p1, p3, 1000);
        CHECK(result.getX() == doctest::Approx(1e308 - 500).epsilon(0.001));
        CHECK(result.getY() == doctest::Approx(1e308 - 500).epsilon(0.001));

        Point p4(2000000000, 123456789);
        Point p5(987654321, -23423842312);
        CHECK(p4.distance(p5) == doctest::Approx(23569050441.749779).epsilon(0.001));
        CHECK(p5.distance(p4) == doctest::Approx(2.35691e+10).epsilon(0.001));

        SUBCASE("Distance method - Points with extremely small values")
        {
            Point p1(DBL_MIN, DBL_MIN);
            Point p2(2 * DBL_MIN, 2 * DBL_MIN);

            CHECK(p1.distance(p2) == doctest::Approx(sqrt(2) * DBL_MIN).epsilon(0.001));
        }

        SUBCASE("Move Towards method - Points close to the limits of double type")
        {
            Point p1(DBL_MAX, DBL_MAX);
            Point p2(DBL_MAX - 1e10, DBL_MAX - 1e10);
            double dist = 1e10;

            Point result = Point::moveTowards(p1, p2, dist);
            CHECK(result.getX() == doctest::Approx(DBL_MAX - dist / sqrt(2)).epsilon(0.001));
            CHECK(result.getY() == doctest::Approx(DBL_MAX - dist / sqrt(2)).epsilon(0.001));
        }
    }
}

TEST_CASE("Testing Character Class and Derived Classes")
{
    SUBCASE("Constructors dont throw exceptions")
    {
        Point location(0, 0);
        CHECK_NOTHROW(Cowboy cowboy("John", location));
        CHECK_NOTHROW(YoungNinja Yninja("John", location));
        CHECK_NOTHROW(TrainedNinja Tninja("John", location));
        CHECK_NOTHROW(OldNinja Oninja("John", location));
    }

    SUBCASE("Cowboy")
    {
        Cowboy cowboy("John", Point(0, 0));
        CHECK_NOTHROW(cowboy.isAlive());
        CHECK_NOTHROW(cowboy.getName());
        CHECK_NOTHROW(cowboy.hasboolets());
        CHECK_NOTHROW(cowboy.reload());
        CHECK_NOTHROW(cowboy.hit(1));
        CHECK_NOTHROW(cowboy.print());
        CHECK_NOTHROW(cowboy.getLocation());

        TrainedNinja Tninja("Bob", Point(0, 1));
        CHECK_NOTHROW(cowboy.shoot(&Tninja));
        CHECK_NOTHROW(cowboy.distance(&Tninja));

        CHECK(cowboy.isAlive() == true);
        CHECK(cowboy.getName() == "John");
        CHECK(cowboy.getLocation().getX() == 0);
        CHECK(cowboy.getLocation().getY() == 0);
        CHECK(cowboy.hasboolets() == true);
        CHECK(cowboy.distance(&Tninja) == 1);


        // cowboy cant shoot himself
        CHECK_THROWS_AS(cowboy.shoot(&cowboy), std::invalid_argument);

        //cowboy cant shoot dead target
        Cowboy t1("John",Point(0,0));
        t1.hit(110);
        CHECK_THROWS_AS(cowboy.shoot(&t1),std::invalid_argument);
        cowboy.reload();

        SUBCASE("Correct reload")
        {
            Cowboy cowboy1("John", Point(0, 0));
            TrainedNinja enemy("enemy", Point(0, 0));
            CHECK(cowboy1.hasboolets() == true);
            for (int i = 0; i < 6; i++)
            {
                cowboy1.shoot(&enemy);
            }

            CHECK(cowboy1.hasboolets() == false);
            cowboy1.reload();
            CHECK(cowboy1.hasboolets() == true);

            // check that reload resets the amount of bullets to 6 and does not increase it each time by 6
            for (int i = 0; i < 10; i++)
            {
                cowboy1.reload();
            }

            for (int i = 0; i < 6; i++)
            {
                cowboy1.shoot(&enemy);
            }

            CHECK(cowboy1.hasboolets() == false);
            CHECK(enemy.isAlive() == false); 
        }

        SUBCASE("Cowboy Shoot")
        {
            TrainedNinja ninja("Yuki", Point(0, 5));
            cowboy.shoot(&ninja);

            CHECK(ninja.isAlive() == true);
            CHECK(cowboy.hasboolets() == true);

            for (int i = 0; i < 5; ++i)
            {
                cowboy.shoot(&ninja);
            }

            CHECK(cowboy.hasboolets() == false);
            CHECK(ninja.isAlive() == true);

            cowboy.reload();
            CHECK(cowboy.hasboolets() == true);

            for (int i = 0; i < 6; ++i)
            {
                cowboy.shoot(&ninja);
            }

            CHECK(cowboy.hasboolets() == false);
            CHECK(ninja.isAlive() == false);
            cowboy.reload();
        }

        SUBCASE("Cowboy Hitpoints and hit function")
        {
            Cowboy cowboy2("Bob", Point(10, -37));
            CHECK(cowboy2.isAlive() == true);
            cowboy2.hit(109);
            CHECK(cowboy2.isAlive() == true);
            cowboy2.hit(1);
            CHECK(cowboy2.isAlive() == false);

            Cowboy cowboy3("Bob", Point(5, 5));
            Cowboy cowboy4("Bob2", Point(8, 9));

            for (int i = 0; i < 6; i++) // deal 60 damage to each other, 50 hp left, reload needed
            {
                cowboy3.shoot(&cowboy4);
                cowboy4.shoot(&cowboy3);
            }

            CHECK(cowboy3.isAlive() == true);
            CHECK(cowboy4.isAlive() == true);
            cowboy3.reload();
            cowboy4.reload();

            for (int i = 0; i < 4; i++) // deal 40 damage to each other, 10 hp left
            {
                cowboy3.shoot(&cowboy4);
                cowboy4.shoot(&cowboy3);
            }

            CHECK(cowboy3.isAlive() == true);
            CHECK(cowboy4.isAlive() == true);

            cowboy3.shoot(&cowboy4);
            CHECK(cowboy4.isAlive() == false);
            cowboy3.hit(10);
            CHECK(cowboy3.isAlive() == false);
        }

        SUBCASE("Cowboy duel")
        {
            Cowboy c1("John", Point(3, 2));
            Cowboy c2("Bob", Point(-2, 1));
            for (int i = 0; i < 11; i++)
            {
                if (c1.hasboolets() == false || c2.hasboolets() == false)
                {
                    c1.reload();
                    c2.reload();
                }
                c1.shoot(&c2);
                c2.shoot(&c1);
            }

            // c1 should win because he attacked first
            CHECK(c1.isAlive() == true);
            CHECK(c2.isAlive() == false);
        }
    }

    SUBCASE("Ninja and Derived classes functions")
    {
        YoungNinja young_ninja("Yuki", Point(0, 0));
        TrainedNinja trained_ninja("Toshi", Point(0, 5));
        OldNinja old_ninja("Osamu", Point(0, 10));


        // ninjas cannot slash themselves
        CHECK_THROWS_AS(young_ninja.slash(&young_ninja),std::invalid_argument);
        CHECK_THROWS_AS(trained_ninja.slash(&trained_ninja),std::invalid_argument);
        CHECK_THROWS_AS(old_ninja.slash(&old_ninja),std::invalid_argument);

        // ninjas cannot slash dead target
        Cowboy t1("John",Point(0,0));
        Cowboy t2("John",Point(0,5));
        Cowboy t3("John",Point(0,10));
        t1.hit(110);
        t2.hit(110);
        t3.hit(110);
        CHECK_THROWS_AS(young_ninja.slash(&t1),std::invalid_argument);
        CHECK_THROWS_AS(trained_ninja.slash(&t2),std::invalid_argument);
        CHECK_THROWS_AS(old_ninja.slash(&t3),std::invalid_argument);

        CHECK_NOTHROW(young_ninja.isAlive());
        CHECK_NOTHROW(trained_ninja.isAlive());
        CHECK_NOTHROW(old_ninja.isAlive());

        CHECK(young_ninja.isAlive() == true);
        CHECK(trained_ninja.isAlive() == true);
        CHECK(old_ninja.isAlive() == true);

        CHECK_NOTHROW(young_ninja.hit(1));
        CHECK_NOTHROW(trained_ninja.hit(1));
        CHECK_NOTHROW(old_ninja.hit(1));

        CHECK_NOTHROW(young_ninja.getName());
        CHECK_NOTHROW(trained_ninja.getName());
        CHECK_NOTHROW(old_ninja.getName());

        CHECK(young_ninja.getName() == "Yuki");
        CHECK(trained_ninja.getName() == "Toshi");
        CHECK(old_ninja.getName() == "Osamu");

        CHECK_EQ(young_ninja.getLocation().getX(), 0);
        CHECK_EQ(trained_ninja.getLocation().getX(), 0);
        CHECK_EQ(old_ninja.getLocation().getX(), 0);
        CHECK_EQ(young_ninja.getLocation().getY(), 0);
        CHECK_EQ(trained_ninja.getLocation().getY(), 5);
        CHECK_EQ(old_ninja.getLocation().getY(), 10);

        CHECK_NOTHROW(young_ninja.print());
        CHECK_NOTHROW(trained_ninja.print());
        CHECK_NOTHROW(old_ninja.print());

        TrainedNinja enemy_ninja("enemy", Point(0, 5));
        CHECK_NOTHROW(young_ninja.move(&enemy_ninja));
        CHECK_NOTHROW(trained_ninja.move(&enemy_ninja));
        CHECK_NOTHROW(old_ninja.move(&enemy_ninja));
        CHECK_NOTHROW(young_ninja.slash(&enemy_ninja));
        CHECK_NOTHROW(trained_ninja.slash(&enemy_ninja));
        CHECK_NOTHROW(old_ninja.slash(&enemy_ninja));

        TrainedNinja dest_ninja("enemy", Point(2, 6));
        CHECK_NOTHROW(young_ninja.move(&dest_ninja));
        CHECK_NOTHROW(trained_ninja.move(&dest_ninja));
        CHECK_NOTHROW(old_ninja.move(&dest_ninja));
        CHECK_NOTHROW(young_ninja.distance(&dest_ninja));
        CHECK_NOTHROW(trained_ninja.distance(&dest_ninja));
        CHECK_NOTHROW(old_ninja.distance(&dest_ninja));

        young_ninja.move(&dest_ninja);
        trained_ninja.move(&dest_ninja);
        old_ninja.move(&dest_ninja);
        CHECK(young_ninja.distance(&dest_ninja) == 0);
        CHECK(trained_ninja.distance(&dest_ninja) == 0);
        CHECK(old_ninja.distance(&dest_ninja) == 0);
    }

    SUBCASE("Ninja and Derived Classes")
    {
        YoungNinja young_ninja("Yuki", Point(0, 0));
        TrainedNinja trained_ninja("Toshi", Point(0, 5));
        OldNinja old_ninja("Osamu", Point(0, 10));

        SUBCASE("Ninja Move and Slash")
        {
            Cowboy cowboy("John", Point(0, 0));

            young_ninja.move(&cowboy);
            trained_ninja.move(&cowboy);
            old_ninja.move(&cowboy);

            young_ninja.slash(&cowboy);
            trained_ninja.slash(&cowboy);
            old_ninja.slash(&cowboy);

            CHECK(old_ninja.distance(&cowboy) > 1);
            CHECK(cowboy.isAlive() == true); // only 80 damage was done, old ninja is too far away

            for (int i = 0; i < 10; i++) // old ninja slash doesnt do damage cause he is too far away
            {
                old_ninja.slash(&cowboy);
            }

            CHECK(cowboy.isAlive() == true);

            old_ninja.move(&cowboy);
            
            old_ninja.slash(&cowboy);
            CHECK(cowboy.isAlive() == false);
        }
    }

    SUBCASE("Ninja and Derived classes hitpoints and damage")
    {
        YoungNinja young_ninja1("Yuki", Point(0, 0));
        TrainedNinja trained_ninja1("Toshi", Point(0, 5));
        OldNinja old_ninja1("Osamu", Point(0, 10));

        young_ninja1.hit(99);
        trained_ninja1.hit(119);
        old_ninja1.hit(149);
        // all should have 1 hp

        CHECK(young_ninja1.isAlive() == true);
        CHECK(trained_ninja1.isAlive() == true);
        CHECK(old_ninja1.isAlive() == true);

        young_ninja1.hit(1);
        trained_ninja1.hit(1);
        old_ninja1.hit(1);

        CHECK(young_ninja1.isAlive() == false);
        CHECK(trained_ninja1.isAlive() == false);
        CHECK(old_ninja1.isAlive() == false);

        SUBCASE("young ninja damage")
        {
            YoungNinja young_ninja("Yuki", Point(0, 0));
            Cowboy target1("enemy", Point(0, 5)); // has 110 hp
            target1.hit(69);                      // now has 41 hp

            young_ninja.move(&target1);
            young_ninja.slash(&target1); // should have 1 hp now
            CHECK(target1.isAlive() == true);
            target1.hit(1);
            CHECK(target1.isAlive() == false);
        }

        SUBCASE("trained ninja damage")
        {
            TrainedNinja trained_ninja("Toshi", Point(0, 5));
            Cowboy target2("enemy", Point(0, 5)); // has 110 hp
            target2.hit(69);                      // now has 41 hp

            trained_ninja.move(&target2);
            trained_ninja.slash(&target2); // should have 1 hp now
            CHECK(target2.isAlive() == true);
            target2.hit(1);
            CHECK(target2.isAlive() == false);
        }

        SUBCASE("old ninja damage")
        {
            OldNinja old_ninja("Osamu", Point(0, 10));
            Cowboy target3("enemy", Point(0, 5)); // has 110 hp
            target3.hit(69);                      // now has 41 hp

            old_ninja.move(&target3);
            old_ninja.slash(&target3); // should have 1 hp now
            CHECK(target3.isAlive() == true);
            target3.hit(1);
            CHECK(target3.isAlive() == false);
        }
    }

    SUBCASE("Ninja and Derived classes speed")
    {
        SUBCASE("young ninja speed")
        {
            YoungNinja young_ninja("Yuki", Point(0, 0));
            Cowboy target1("John", Point(0, 15));
            CHECK(young_ninja.distance(&target1) == 15);
            young_ninja.move(&target1);
            CHECK(young_ninja.distance(&target1) == 1);
        }

        SUBCASE("trained ninja speed")
        {
            TrainedNinja trained_ninja("Toshi", Point(0, 0));
            Cowboy target1("John", Point(0, 13));
            CHECK(trained_ninja.distance(&target1) == 13);
            trained_ninja.move(&target1);
            CHECK(trained_ninja.distance(&target1) == 1);
        }

        SUBCASE("old ninja speed")
        {
            OldNinja old_ninja("Osamu", Point(0, 0));
            Cowboy target1("John", Point(0, 9));
            CHECK(old_ninja.distance(&target1) == 9);
            old_ninja.move(&target1);
            CHECK(old_ninja.distance(&target1) == 1);
        }

        SUBCASE("ninjas are able to reach target from far distance")
        {
            YoungNinja young_ninja("Yuki", Point(0, 0));
            TrainedNinja trained_ninja("Toshi", Point(0, 0));
            OldNinja old_ninja("Osamu", Point(0, 0));
            Cowboy target1("John", Point(10000, 10000));
            while (young_ninja.distance(&target1) != 0)
            {
                young_ninja.move(&target1);
            }
            while (old_ninja.distance(&target1) != 0)
            {
                old_ninja.move(&target1);
            }
            while (trained_ninja.distance(&target1) != 0)
            {
                trained_ninja.move(&target1);
            }

            CHECK(young_ninja.distance(&target1) == 0);
            CHECK(old_ninja.distance(&target1) == 0);
            CHECK(trained_ninja.distance(&target1) == 0);
        }

        SUBCASE("Ninjas distance from enemy must be equal or less than 1 to do damage")
        {
            YoungNinja young_ninja("Yuki", Point(0, 0));
            TrainedNinja trained_ninja("Toshi", Point(0, 0));
            OldNinja old_ninja("Osamu", Point(0, 0));
            OldNinja target1("John", Point(0, 5));
            OldNinja target2("John2", Point(0, 0.5));
            OldNinja target3("John3", Point(0.3, -0.1));

            for (int i = 0; i < 2; i++)
            {
                young_ninja.slash(&target1);
                trained_ninja.slash(&target1);
                old_ninja.slash(&target1);
                young_ninja.slash(&target2);
                trained_ninja.slash(&target2);
                old_ninja.slash(&target2);
                young_ninja.slash(&target3);
                trained_ninja.slash(&target3);
                old_ninja.slash(&target3);
            }

            // only target1 should survive because the distance from him is 5 which is greater than 1
            CHECK(target1.isAlive() == true);
            CHECK(target2.isAlive() == false);
            CHECK(target3.isAlive() == false);
            young_ninja.move(&target1);
            trained_ninja.move(&target1);
            old_ninja.move(&target1);

            target1.hit(30);
            for (int i = 0; i < 1; i++)
            {
                young_ninja.slash(&target1);
                trained_ninja.slash(&target1);
                old_ninja.slash(&target1);
            }

            CHECK(target1.isAlive() == false);
        }
    }

    SUBCASE("Ninja duels")
    {
        SUBCASE("Old ninja vs Young ninja")
        {
            YoungNinja young_ninja("Yuki", Point(0, 0));
            OldNinja old_ninja("Osamu", Point(0, 0));
            while (old_ninja.isAlive() && young_ninja.isAlive())
            {
                old_ninja.slash(&young_ninja);
                young_ninja.slash(&old_ninja);
            }

            // old ninja should win because it has more hp and both of them at the same location so speed has no impact
            CHECK(young_ninja.isAlive() == false);
            CHECK(old_ninja.isAlive() == true);
        }

        SUBCASE("Old ninja vs Trained ninja")
        {
            TrainedNinja trained_ninja("Yuki", Point(0, 0));
            OldNinja old_ninja("Osamu", Point(0, 0));
            while (old_ninja.isAlive() && trained_ninja.isAlive())
            {
                old_ninja.slash(&trained_ninja);
                trained_ninja.slash(&old_ninja);
            }

            // old ninja should win because it has more hp and both of them at the same location so speed has no impact
            CHECK(trained_ninja.isAlive() == false);
            CHECK(old_ninja.isAlive() == true);
        }

        SUBCASE("Trained ninja vs Young ninja")
        {
            YoungNinja young_ninja("Yuki", Point(0, 0));
            TrainedNinja trained_ninja("Yuki", Point(0, 0));
            while (trained_ninja.isAlive() && young_ninja.isAlive())
            {
                trained_ninja.slash(&young_ninja);
                young_ninja.slash(&trained_ninja);
            }

            // trained ninja should win because it has more hp, he attacked first and both of them at the same location so speed has no impact
            CHECK(young_ninja.isAlive() == false);
            CHECK(trained_ninja.isAlive() == true);
        }
    }
}

TEST_CASE("Testing Team")
{
    SUBCASE("Constructor")
    {
        TrainedNinja *trained_ninja = new TrainedNinja("Bob", Point(3, 5));
        CHECK_NOTHROW(Team A(trained_ninja));
        Cowboy *cowboy = new Cowboy("John", Point(0, 0));
        Team team_b(cowboy);
        CHECK_THROWS_AS(Team team_c(cowboy), std::invalid_argument); // cowboy already in team b
        CHECK_NOTHROW(team_b.print());
        CHECK_NOTHROW(team_b.stillAlive());
        CHECK(team_b.stillAlive() > 0);
    }

    SUBCASE("Team functions")
    {
        Cowboy *cowboy = new Cowboy("John", Point(0, 0));
        YoungNinja young_ninja("n1", Point(1, 3));
        OldNinja old_ninja("n2", Point(2, 3));
        TrainedNinja *trained_ninja = new TrainedNinja("n3", Point(1, -0));

        Team team_a(cowboy);
        CHECK_NOTHROW(team_a.add(&young_ninja));
        team_a.add(&old_ninja);
        Team team_b(trained_ninja);
        CHECK_THROWS_AS(team_b.add(&old_ninja), std::invalid_argument); // old ninja is already in team a

        CHECK_NOTHROW(team_a.attack(&team_b));
        CHECK_NOTHROW(team_b.attack(&team_a));

        while (team_a.stillAlive() > 0 && team_b.stillAlive() > 0)
        {
            team_a.attack(&team_b);
            team_b.attack(&team_a);
        }

        // team a should win
        CHECK(team_a.stillAlive() > 0);
        CHECK(team_b.stillAlive() == 0);
    }

    SUBCASE("Team fight")
    {
        OldNinja *n1 = new OldNinja("n1", Point(3, 14));
        YoungNinja *n2 = new YoungNinja("n2", Point(2, 0));
        TrainedNinja *n3 = new TrainedNinja("n3", Point(7.15, 14.3));
        Cowboy *n4 = new Cowboy("n4", Point(3, 3));
        OldNinja *n5 = new OldNinja("n5", Point(0, 5));
        YoungNinja *n6 = new YoungNinja("n6", Point(30, 1));
        TrainedNinja *n7 = new TrainedNinja("n7", Point(3, -14));
        Cowboy *n8 = new Cowboy("n8", Point(-1, -7));

        Team team1(n1);
        Team team2(n5);
        team1.add(n2);
        team1.add(n3);
        team1.add(n4);
        team2.add(n6);
        team2.add(n7);
        team2.add(n8);

        while (team1.stillAlive() > 0 && team2.stillAlive() > 0)
        {
            team1.attack(&team2);
            team2.attack(&team1);
        }

        bool teamwon = (team1.stillAlive() > 0) || (team2.stillAlive() > 0);
        CHECK(teamwon == true);
    }

    SUBCASE("full team and stillalive function")
    {
        Cowboy *cowboy = new Cowboy("John", Point(0, 0));
        Team team_a(cowboy);
        Cowboy *cowboy2 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy3 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy4 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy5 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy6 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy7 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy8 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy9 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy10 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy11 = new Cowboy("John", Point(0, 0));
        Cowboy *cowboy12 = new Cowboy("John", Point(0, 0));
        CHECK_THROWS_AS(team_a.add(cowboy), std::invalid_argument); // already inside team
        team_a.add(cowboy2);
        team_a.add(cowboy3);
        team_a.add(cowboy4);
        team_a.add(cowboy5);
        team_a.add(cowboy6);
        team_a.add(cowboy7);
        team_a.add(cowboy8);
        team_a.add(cowboy9);
        team_a.add(cowboy10);
        CHECK(team_a.stillAlive() == 10);
        CHECK_THROWS_AS(team_a.add(cowboy11), std::invalid_argument); // team is full
        CHECK_THROWS_AS(team_a.add(cowboy12), std::invalid_argument); // team is full
        CHECK(team_a.stillAlive() == 10);
        Team team_b(cowboy11);
        team_a.attack(&team_b); // 100 damage was dealt to cowboy11, he has 110 hp
        CHECK(team_b.stillAlive() == 1);
        team_a.attack(&team_b);
        CHECK(team_b.stillAlive() == 0);
        team_b.add(new YoungNinja("Yogi", Point(64, 57)));
        team_b.add(new YoungNinja("Yogi2", Point(64, 57)));
        CHECK(team_b.stillAlive() == 2);

        // all cowboys together deal 100 damage (10 each), so each time team_a attack team_b a young ninja should die because it has 100 hp
        team_a.attack(&team_b);
        CHECK(team_b.stillAlive() == 1);
        team_a.attack(&team_b);
        CHECK(team_b.stillAlive() == 0);

        team_b.add(new YoungNinja("Yogi3", Point(4, 7)));
        while (team_a.stillAlive() > 0) // eventually all team_a dies
        {
            team_b.attack(&team_a);
        }

        CHECK(team_a.stillAlive() == 0);
        CHECK(team_b.stillAlive() == 1);
    }

    SUBCASE("Team - Attack when all characters are dead")
    {
        Cowboy leader1("CowboyLeader1", Point(7, 14));
        Team team1(&leader1);

        Cowboy leader2("CowboyLeader2", Point(-5, 3));
        Team team2(&leader2);

        TrainedNinja ninja("TrainedNinja", Point(-7, -10));
        team2.add(&ninja);

        // Kill the leader of team1
        leader1.hit(110);

        for (int i = 0; i < 50; i++)
        {
            team1.attack(&team2);
        }

        // no damage should have been dealt
        CHECK(leader2.isAlive() == true);
        CHECK(ninja.isAlive() == true);
        CHECK(team2.stillAlive() == 2);
    }

    SUBCASE("Team - closest ally to leader is the one being attacked")
    {
        Cowboy leader1("CowboyLeader1", Point(0, 0));
        Team team1(&leader1);

        Cowboy leader2("CowboyLeader2", Point(0, 0));
        Team team2(&leader2);

        Cowboy c1("John", Point(3, 4));
        Cowboy c2("John", Point(3, 4));
        Cowboy c3("John", Point(8, 15));
        Cowboy c4("John", Point(8, 15));
        Cowboy c5("John", Point(30, 40));
        Cowboy c6("John", Point(30, 40));
        team1.add(&c1);
        team1.add(&c3);
        team1.add(&c5);
        team2.add(&c2);
        team2.add(&c4);
        team2.add(&c6);
        // each team has 4 cowboys now which means a team attack will deal 40 damage
        CHECK(team1.stillAlive() == 4);
        CHECK(team2.stillAlive() == 4);

        for (int i = 0; i < 3; i++)
        {
            team1.attack(&team2);
            team2.attack(&team1);
        }

        // cowboys c1 and c2 should have died since they are closest to their leaders
        CHECK(c1.isAlive() == false);
        CHECK(c2.isAlive() == false);
        CHECK(team1.stillAlive() == 3);
        CHECK(team2.stillAlive() == 3);

        for (int i = 0; i < 4; i++)
        {
            team1.attack(&team2);
            team2.attack(&team1);
        }

        // cowboys c3 and c4 should have died since they are closest to their leaders
        CHECK(c3.isAlive() == false);
        CHECK(c4.isAlive() == false);
        CHECK(team1.stillAlive() == 2);
        CHECK(team2.stillAlive() == 2);
    }

    SUBCASE("New leader is chosen")
    {
        Cowboy leader1("CowboyLeader1", Point(0, 0));
        Team team1(&leader1);

        Cowboy leader2("CowboyLeader2", Point(0, 0));
        Team team2(&leader2);

        Cowboy c1("John", Point(3, 4));
        Cowboy c2("John", Point(3, 4));
        Cowboy c3("John", Point(8, 15));
        Cowboy c4("John", Point(8, 15));
        Cowboy c5("John", Point(30, 40));
        Cowboy c6("John", Point(30, 40));
        team1.add(&c1);
        team1.add(&c3);
        team1.add(&c5);
        team2.add(&c2);
        team2.add(&c4);
        team2.add(&c6);

        CHECK(team1.stillAlive() == 4);
        CHECK(team2.stillAlive() == 4);

        leader1.hit(110);
        leader2.hit(110);

        CHECK(team1.stillAlive() == 3);
        CHECK(team2.stillAlive() == 3);

        for (int i = 0; i < 5; i++)
        {
            team1.attack(&team2);
            team2.attack(&team1);
        }

        // cowboys c1 and c2 should have been chosen to be the new leaders so they should be alive now
        CHECK(c1.isAlive() == true);
        CHECK(c2.isAlive() == true);

        // cowboys c3 and c4 should have died since they are closest to their new leaders
        CHECK(c3.isAlive() == false);
        CHECK(c4.isAlive() == false);
        CHECK(team1.stillAlive() == 2);
        CHECK(team2.stillAlive() == 2);
    }

    SUBCASE("Team - Attack when all bullets are used")
    {
        Cowboy leader1("CowboyLeader1", Point(2, 5));
        Team team1(&leader1);

        Cowboy leader2("CowboyLeader2", Point(3.14, -17));
        Team team2(&leader2);

        // Bring leader2's hit points down to 70
        leader2.hit(40);

        // Team1 leader shoots all bullets
        for (int i = 0; i < 6; ++i)
        {
            leader1.shoot(&leader2);
        }

        // Team1 attacks Team2
        team1.attack(&team2);

        // Leader1 should not cause any more damage, he should reload
        CHECK(leader2.isAlive() == true);
        CHECK(team2.stillAlive() == 1);
    }

    SUBCASE("Team - Attack when ninjas are not close enough")
    {
        OldNinja leader1("OldNinjaLeader", Point(0, 8));
        Team team1(&leader1);

        TrainedNinja ninja1("TrainedNinja1", Point(0, 0));
        team1.add(&ninja1);

        Cowboy leader2("CowboyLeader2", Point(0, 24));
        Team team2(&leader2);

        // Ninjas should move if they are too far away, after 2 attacks(in this case) reach to the enemy leader and start dealing damage
        for (int i = 0; i < 4; i++)
        {
            team1.attack(&team2);
        }

        CHECK(leader2.isAlive() == false);
        CHECK(team2.stillAlive() == 0);
    }
}

TEST_CASE("Testing Team2 and SmartTeam")
{
    SUBCASE("Constructors")
    {
        CHECK_NOTHROW(Team2 t(new Cowboy("John", Point(-5, 17))));
        Cowboy *c = new Cowboy("John", Point(3, 3));
        Team2 t1(c);
        CHECK_NOTHROW(t1.stillAlive());
        CHECK(t1.stillAlive() == 1);
        CHECK_NOTHROW(t1.print());

        CHECK_NOTHROW(SmartTeam st(new TrainedNinja("Bob", Point(7, -13))));
        YoungNinja *n = new YoungNinja("Name", Point(0.3, -15.7));
        SmartTeam st2(n);
        CHECK_NOTHROW(st2.stillAlive());
        CHECK(st2.stillAlive() == 1);
        CHECK_NOTHROW(st2.print());
    }

    SUBCASE("Team2 functions")
    {
        Cowboy *cowboy = new Cowboy("John", Point(0, 0));
        YoungNinja young_ninja("n1", Point(1, 3));
        OldNinja old_ninja("n2", Point(2, 3));
        TrainedNinja *trained_ninja = new TrainedNinja("n3", Point(1, -0));

        Team2 team_a(cowboy);
        CHECK_NOTHROW(team_a.add(&young_ninja));
        team_a.add(&old_ninja);
        Team2 team_b(trained_ninja);
        CHECK_THROWS_AS(team_b.add(&old_ninja), std::invalid_argument); // old ninja is already in team a

        CHECK_NOTHROW(team_a.attack(&team_b));
        CHECK_NOTHROW(team_b.attack(&team_a));

        while (team_a.stillAlive() > 0 && team_b.stillAlive() > 0)
        {
            team_a.attack(&team_b);
            team_b.attack(&team_a);
        }

        // team a should win
        CHECK(team_a.stillAlive() > 0);
        CHECK(team_b.stillAlive() == 0);
    }

    SUBCASE("SmartTeam functions")
    {
        Cowboy *cowboy = new Cowboy("John", Point(0, 0));
        YoungNinja young_ninja("n1", Point(1, 3));
        OldNinja old_ninja("n2", Point(2, 3));
        TrainedNinja *trained_ninja = new TrainedNinja("n3", Point(1, -0));

        SmartTeam team_a(cowboy);
        CHECK_NOTHROW(team_a.add(&young_ninja));
        team_a.add(&old_ninja);
        SmartTeam team_b(trained_ninja);
        CHECK_THROWS_AS(team_b.add(&old_ninja), std::invalid_argument); // old ninja is already in team a

        CHECK_NOTHROW(team_a.attack(&team_b));
        CHECK_NOTHROW(team_b.attack(&team_a));

        while (team_a.stillAlive() > 0 && team_b.stillAlive() > 0)
        {
            team_a.attack(&team_b);
            team_b.attack(&team_a);
        }

        // team a should win 
        CHECK(team_a.stillAlive() > 0);
        CHECK(team_b.stillAlive() == 0);
    }
    SUBCASE("Team2 cannot have more than 10 warriors")
    {
        Cowboy *leader = new Cowboy("John",Point(200,5));
        Team2 team(leader);

        for (int i = 0; i < 9; ++i)
        {
            OldNinja *warrior = new OldNinja("name", Point(3.14,2.71));
            team.add(warrior);
        }
        Cowboy *cowboy2 = new Cowboy("John", Point(0, 0));
        CHECK_THROWS_AS(team.add(cowboy2), std::invalid_argument); // team is full
        CHECK(team.stillAlive() == 10);
    }

    SUBCASE("SmartTeam cannot have more than 10 warriors")
    {
        Cowboy *leader = new Cowboy("John",Point(200,5));
        SmartTeam team(leader);

        for (int i = 0; i < 9; ++i)
        {
            OldNinja *warrior = new OldNinja("name", Point(3.14,2.71));
            team.add(warrior);
        }
        Cowboy *cowboy2 = new Cowboy("John", Point(0, 0));
        CHECK_THROWS_AS(team.add(cowboy2), std::invalid_argument); // team is full
        CHECK(team.stillAlive() == 10);
    }

    SUBCASE("Team2 correct targeting")
    {
        Cowboy leader1("CowboyLeader1", Point(0, 0));
        Team2 team1(&leader1);

        Cowboy leader2("CowboyLeader2", Point(0, 0));
        Team2 team2(&leader2);

        Cowboy c1("John", Point(3, 4));
        Cowboy c2("John", Point(3, 4));
        Cowboy c3("John", Point(8, 15));
        Cowboy c4("John", Point(8, 15));
        Cowboy c5("John", Point(30, 40));
        Cowboy c6("John", Point(30, 40));
        team1.add(&c1);
        team1.add(&c3);
        team1.add(&c5);
        team2.add(&c2);
        team2.add(&c4);
        team2.add(&c6);

        // each team has 4 cowboys now which means a team attack will deal 40 damage
        CHECK(team1.stillAlive() == 4);
        CHECK(team2.stillAlive() == 4);

        for (int i = 0; i < 3; i++)
        {
            team1.attack(&team2);
            team2.attack(&team1);
        }

        // leaders were added first then should have been targeted first
        CHECK(leader1.isAlive() == false);
        CHECK(leader2.isAlive() == false);
        CHECK(team1.stillAlive() == 3);
        CHECK(team2.stillAlive() == 3);

        for (int i = 0; i < 4; i++)
        {
            team1.attack(&team2);
            team2.attack(&team1);
        }

        //c1 and c2 were added second so they should have been targeted now
        CHECK(c1.isAlive() == false);
        CHECK(c2.isAlive() == false);
        CHECK(team1.stillAlive() == 2);
        CHECK(team2.stillAlive() == 2);
    }
}
