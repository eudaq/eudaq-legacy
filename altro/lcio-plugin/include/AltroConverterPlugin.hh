/*
 * AltroConverterPlugin.hh
 *
 *  Created on: Sep 30, 2009
 *      Author: caiazza
 */

#ifndef ALTROCONVERTERPLUGIN_HH_
#define ALTROCONVERTERPLUGIN_HH_

#include "eudaq/DataConverterPlugin.hh"
#include "eudaq/RawDataEvent.hh"
#include "eudaq/Exception.hh"
#include "eudaq/Logger.hh"

#include "AltroConverterHelper.hh"

namespace eudaq
{
/** Implementation of the DataConverterPlugin to convert an eudaq::Event
 *  to an lcio::event.
 *
 *  The class is implemented as a singleton because it manly
 *  contains the conversion code, which is in the getLcioEvent()
 *  function. There has to be one static instance, which is
 *  registered at the pluginManager. This is automatically done by the
 *  inherited constructor of the DataConverterPlugin.
 */

class AltroConverterPlugin : public DataConverterPlugin
{

public:
    /** Returns the event converted to lcio. This is the working horse and the
     *  main part of this plugin.
     */
    virtual bool GetLCIOSubEvent( lcio::LCEvent & lcioevent,
						  eudaq::Event const & eudaqevent ) const;

    /** Returns the event converted to eudaq::StandardEvent.
     *  Only contains the primitive implementation for the dummy StandardEvent.
     */
    virtual bool GetStandardSubEvent( StandardEvent & standardevent,
				      eudaq::Event const & eudaqevent ) const;


    /** Nested helper class: Exception which is thrown in case of bad data block
     */
    class BadDataBlockException : public std::exception
    {
     protected:
	BadDataBlockException(){}
	std::string message ;

     public:
	virtual ~BadDataBlockException() throw() {}
	BadDataBlockException( const std::string& text ){ message = "BadDataBlockException: " + text ;}
	virtual const char* what() const  throw() { return  message.c_str() ; }
    };

    /** The empty destructor. Need to add it to make it virtual.
     */
    virtual ~AltroConverterPlugin();

protected:
    /** The private constructor. The only time it is called is when the
     *  one single instance is created, which lives within the object.
     *  It calls the DataConverterPlugin constructor with the
     *  according event type string. This automatically registers
     *  the plug-in to the plug-in manager.
     */
    AltroConverterPlugin() : DataConverterPlugin("Altro"){}


private:
    /** The one single instance of the AltroConverterPlugin.
     *  It has to be created below.
     */

    mutable std::vector<unsigned char>::const_iterator _eventheaderstart;
    mutable AltroEventHeader* _eventheader;

    mutable AltroRCUBlock* _RCUBlock;

    static AltroConverterPlugin const m_instance;
};

}

#endif /* ALTROCONVERTERPLUGIN_HH_ */
