// MCNP5/dagmc/test/test_TallyManager.cpp
#include <vector>
#include <map>

// #include "moab/CartVect.hpp"

#include "../TallyManager.hpp"
#include "gtest/gtest.h"

//---------------------------------------------------------------------------//
// TEST FIXTURES
//---------------------------------------------------------------------------//
class TallyManagerTest : public ::testing::Test
{
  protected:
    // initialize variables for each test
    virtual void SetUp()
    {
        manager = new TallyManager();
        
        particle = 1;  
        x = -1.4;
        y = 0.0;
        z = 9.8;
        particle_energy = 1.9; 
        particle_weight = 1.1;
        cell_id = 8;
    }

    // deallocate memory resources
    virtual void TearDown()
    {
        delete manager;
    }

  protected:

   TallyManager* manager;
   unsigned int particle;
   double x;
   double y;
   double z;
   double particle_energy;
   double particle_weight;
   int cell_id;
};
//---------------------------------------------------------------------------//
// SIMPLE TESTS
//---------------------------------------------------------------------------//
// Test the event functions
TEST(TallyEventTest, TestConstructor)
{
   TallyManager manager;
   TallyEvent event = manager.getEvent();
   EXPECT_EQ(TallyEvent::NONE, event.type);
}
//---------------------------------------------------------------------------//
TEST(TallyEventTest, TestCollision)
{
   TallyManager manager;
   // Refer
   const TallyEvent& event = manager.getEvent();

   unsigned int particle = 1;  
   double x = -1.4;
   double y = 0.0;
   double z = 9.8;
   double particle_energy = 1.9; 
   double particle_weight = 1.1;
   double total_cross_section = 2.6;
   int cell_id = 8;

   bool isSet = manager.setCollisionEvent(particle, x, y, z, 
                             particle_energy,
                             particle_weight,
			     total_cross_section,
			     cell_id);

   EXPECT_TRUE(isSet);
   EXPECT_EQ(TallyEvent::COLLISION, event.type);
   EXPECT_EQ(particle, event.particle);
   EXPECT_EQ(cell_id, event.current_cell);
   EXPECT_DOUBLE_EQ(0.0, event.track_length);
   EXPECT_DOUBLE_EQ(x, event.position[0]);	
   EXPECT_DOUBLE_EQ(y, event.position[1]);
   EXPECT_DOUBLE_EQ(z, event.position[2]);
   
   EXPECT_DOUBLE_EQ(0.0, event.direction[0]);
   EXPECT_DOUBLE_EQ(0.0, event.direction[1]);
   EXPECT_DOUBLE_EQ(0.0, event.direction[2]);
  

   EXPECT_DOUBLE_EQ(total_cross_section, event.total_cross_section);
   EXPECT_DOUBLE_EQ(particle_energy, event.particle_energy);
   EXPECT_DOUBLE_EQ(particle_weight, event.particle_weight);

}

//---------------------------------------------------------------------------//
TEST(TallyEventTest, TestTrack)
{
   TallyManager manager;
   const TallyEvent& event = manager.getEvent();
 
   unsigned int particle = 2;
   double x = -1.5;
   double y = 0.0;
   double z = 9.9;
   double u = 1.4;
   double v = 0.0;
   double w = -9.1;
   double particle_energy = 9.1; 
   double particle_weight = 2.2;
   double track_length = 2.6;
   int cell_id = 6;

   bool isSet = manager.setTrackEvent(particle, x, y, z, u, v, w,
                                      particle_energy,
				      particle_weight,
				      track_length,
				      cell_id);

   
   EXPECT_TRUE(isSet);
   EXPECT_EQ(TallyEvent::TRACK, event.type);
   EXPECT_EQ(particle, event.particle);
   EXPECT_EQ(cell_id, event.current_cell);
   EXPECT_DOUBLE_EQ(track_length, event.track_length);
   EXPECT_DOUBLE_EQ(x, event.position[0]);	
   EXPECT_DOUBLE_EQ(y, event.position[1]);
   EXPECT_DOUBLE_EQ(z, event.position[2]);
   
   EXPECT_NEAR(0.152057, event.direction[0], 1E-6);
   EXPECT_DOUBLE_EQ(0.0, event.direction[1]);
   EXPECT_NEAR(-0.988372, event.direction[2], 1E-6);

   EXPECT_DOUBLE_EQ(0.0, event.total_cross_section);
   EXPECT_DOUBLE_EQ(particle_energy, event.particle_energy);
   EXPECT_DOUBLE_EQ(particle_weight, event.particle_weight);

}
//---------------------------------------------------------------------------//
// FIXTURE-BASED TESTS: TallyManagerTest
//---------------------------------------------------------------------------//
TEST_F(TallyManagerTest, TestBadCollision)
{
   double total_cross_section = -2.5;

   bool isSet = manager->setCollisionEvent(particle, x, y, z, 
                             particle_energy,
                             particle_weight,
			     total_cross_section,
			     cell_id);

   EXPECT_FALSE(isSet);
}
//---------------------------------------------------------------------------//
TEST_F(TallyManagerTest, TestBadTrack)
{
   double u = 1.4;
   double v = 0.0;
   double w = -9.1;
   double track_length = -2.7;

   bool isSet = manager->setTrackEvent(particle, x, y, z, u, v, w,
                                      particle_energy,
				      particle_weight,
				      track_length,
				      cell_id);
   EXPECT_FALSE(isSet);
}
//---------------------------------------------------------------------------//
// end of MCNP5/dagmc/test/test_TallyManager.cpp
