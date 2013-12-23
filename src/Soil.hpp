#ifndef __SOIL_HPP__
#define __SOIL_HPP__


#include <boost/shared_ptr.hpp>
#include <dodge/EEvent.hpp>
#include <dodge/Sprite.hpp>
#include "dodge/xml/xml.hpp"


class Soil : public Dodge::Sprite {
   public:
      Soil(const Dodge::XmlNode data);
      Soil(const Soil& copy);
      Soil(const Soil& copy, long name);

      virtual size_t getSize() const;
      virtual Soil* clone() const;

      virtual void onEvent(const Dodge::EEvent* event);
      virtual void assignData(const Dodge::XmlNode data);

      virtual ~Soil();
};

typedef boost::shared_ptr<Soil> pSoil_t;


#endif
