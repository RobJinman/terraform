#include <memory>
#include <dodge/math/math.hpp>
#include <dodge/EEntityCollision.hpp>
#include "Platform.hpp"


using namespace std;
using namespace Dodge;
using namespace boost;


//===========================================
// Platform::Platform
//===========================================
Platform::Platform(const XmlNode data)
   : Asset(internString("Platform")),
     Entity(data.firstChild().firstChild().firstChild()),
     PhysicalSprite(data.firstChild()) {

   try {
      XML_NODE_CHECK(data, Platform);
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Platform; ");
   }
}

//===========================================
// Platform::Platform
//===========================================
Platform::Platform(const Platform& copy)
   : Asset(internString("Platform")),
     Entity(copy),
     PhysicalSprite(copy) {}

//===========================================
// Platform::Platform
//===========================================
Platform::Platform(const Platform& copy, long name)
   : Asset(internString("Platform")),
     Entity(copy, name),
     PhysicalSprite(copy, name) {}

//===========================================
// Platform::getSize
//===========================================
size_t Platform::getSize() const {
   return sizeof(Platform)
      - sizeof(PhysicalSprite<Box2dPhysics>)
      + PhysicalSprite<Box2dPhysics>::getSize()
      + sizeof(std::set<Dodge::pEntity_t>) + sizeof(pEntity_t) * m_contacts.size()
      + sizeof(std::set<Dodge::pEntity_t>) + sizeof(pEntity_t) * m_pendingAddition.size()
      + sizeof(std::set<Dodge::pEntity_t>) + sizeof(pEntity_t) * m_pendingRemoval.size();
}

//===========================================
// Platform::clone
//===========================================
Platform* Platform::clone() const {
   return new Platform(*this);
}

//===========================================
// Platform::assignData
//===========================================
void Platform::assignData(const XmlNode data) {
   try {
      XML_NODE_CHECK(data, Platform)

      XmlNode node = data.firstChild();
      if (!node.isNull() && node.name() == "PhysicalSprite") {
         PhysicalSprite<Box2dPhysics>::assignData(node);
      }
   }
   catch (XmlException& e) {
      e.prepend("Error parsing XML for instance of class Platform; ");
      throw;
   }
}

//===========================================
// Platform::update
//===========================================
void Platform::update() {
   static long idleStr = internString("idle");

   for (auto i = m_pendingAddition.begin(); i != m_pendingAddition.end(); ++i) {
      if (m_contacts.find(*i) != m_contacts.end()) continue;

      Vec2f targetTransl = (*i)->getTranslation_abs();
      float32_t targetRot = (*i)->getRotation_abs();

      addChild(*i);

      (*i)->setRotation_abs(targetRot);
      (*i)->setTranslation_abs(targetTransl);

      m_contacts.insert(*i);
   }

   for (auto i = m_pendingRemoval.begin(); i != m_pendingRemoval.end(); ++i) {
      if (m_contacts.find(*i) == m_contacts.end()) continue;

      Vec2f targetTransl = (*i)->getTranslation_abs();
      float32_t targetRot = (*i)->getRotation_abs();

      removeChild(*i);

      (*i)->setRotation_abs(targetRot);
      (*i)->setTranslation_abs(targetTransl);

      m_contacts.erase(*i);
   }

   m_pendingAddition.clear();
   m_pendingRemoval.clear();

   playTransformation(idleStr);
   PhysicalSprite::update();
}

//===========================================
// Platform::onEvent
//===========================================
void Platform::onEvent(const EEvent* event) {
   PhysicalSprite::onEvent(event);

   static long entityCollisionStr = internString("entityCollision");
   if (event->getType() != entityCollisionStr) return;

   const EEntityCollision* e = static_cast<const EEntityCollision*>(event);

   // We're in the middle of Box2D's step() function, so
   // can't move anything yet.

   assert(e->entityA);

   assert(e->entityA == getSharedPtr());
   assert(e->entityB != getSharedPtr());

   assert(e->entityB.use_count() > 1);

   if (e->incoming) {
      if (m_contacts.find(e->entityB) == m_contacts.end()) {
         m_pendingAddition.insert(e->entityB);
      }
   }
   else {
      m_pendingRemoval.insert(e->entityB);
   }
}

//===========================================
// Platform::~Platform
//===========================================
Platform::~Platform() {}
