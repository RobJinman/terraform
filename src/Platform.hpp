#ifndef __PLATFORM_HPP__
#define __PLATFORM_HPP__


#include <set>
#include <dodge/PhysicalSprite.hpp>
#include <dodge/Box2dPhysics.hpp>


class Platform : public Dodge::PhysicalSprite<Dodge::Box2dPhysics> {
   public:
      Platform(const Dodge::XmlNode data);
      Platform(const Platform& copy);
      Platform(const Platform& copy, long name);

      virtual size_t getSize() const;
      virtual Platform* clone() const;

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void update();

      virtual void assignData(const Dodge::XmlNode data);

      virtual ~Platform();

   private:
      std::set<Dodge::pEntity_t> m_contacts;
      std::set<Dodge::pEntity_t> m_pendingAddition;
      std::set<Dodge::pEntity_t> m_pendingRemoval;
};


#endif
