#include <vector>
#include <mutex>

#include <omxil_mf/component.hpp>
#include <omxil_mf/port_audio.hpp>
#include <omxil_mf/scoped_log.hpp>

#include "api/consts.hpp"
#include "util/omx_enum_name.hpp"

namespace mf {

port_audio::port_audio(int ind, component *c)
	: port(ind, c),
	mime_type(nullptr), native_render(nullptr),
	flag_error_concealment(OMX_FALSE),
	default_format(-1)
{
	scoped_log_begin;

	set_domain(OMX_PortDomainAudio);
}

port_audio::~port_audio()
{
	scoped_log_begin;
	//do nothing
}

const char *port_audio::get_name() const
{
	return "port_audio";
}

OMX_STRING port_audio::get_mime_type() const
{
	return mime_type;
}

void port_audio::set_mime_type(OMX_STRING v)
{
	mime_type = v;
}

OMX_NATIVE_DEVICETYPE port_audio::get_native_render() const
{
	return native_render;
}

void port_audio::set_native_render(OMX_NATIVE_DEVICETYPE v)
{
	native_render = v;
}

OMX_BOOL port_audio::get_flag_error_concealment() const
{
	return flag_error_concealment;
}

void port_audio::set_flag_error_concealment(OMX_BOOL v)
{
	flag_error_concealment = v;
}

OMX_AUDIO_CODINGTYPE port_audio::get_encoding() const
{
	const OMX_AUDIO_PARAM_PORTFORMATTYPE *f = get_default_format();

	if (f == nullptr) {
		return OMX_AUDIO_CodingUnused;
	} else {
		return f->eEncoding;
	}
}

const OMX_PARAM_PORTDEFINITIONTYPE *port_audio::get_definition() const
{
	scoped_log_begin;

	super::get_definition();

	definition.format.audio.cMIMEType     = mime_type;
	definition.format.audio.pNativeRender = native_render;
	definition.format.audio.bFlagErrorConcealment = flag_error_concealment;
	definition.format.audio.eEncoding     = get_encoding();

	return &definition;
}

const OMX_AUDIO_PARAM_PORTFORMATTYPE *port_audio::get_supported_format(size_t index) const
{
	if (index < 0 || formats.size() <= index) {
		return nullptr;
	}

	return &formats.at(index);
}

OMX_ERRORTYPE port_audio::add_supported_format(const OMX_AUDIO_PARAM_PORTFORMATTYPE *f)
{
	OMX_AUDIO_PARAM_PORTFORMATTYPE fmt;

	if (f == nullptr) {
		return OMX_ErrorBadParameter;
	}

	fmt = *f;
	fmt.nPortIndex = 0;
	fmt.nIndex = 0;
	formats.push_back(fmt);

	return OMX_ErrorNone;
}

OMX_ERRORTYPE port_audio::remove_supported_format(size_t index)
{
	if (index < 0 || formats.size() <= index) {
		return OMX_ErrorBadParameter;
	}

	formats.erase(formats.begin() + index);

	return OMX_ErrorNone;
}

const OMX_AUDIO_PARAM_PORTFORMATTYPE *port_audio::get_default_format() const
{
	return get_supported_format(default_format);
}

OMX_ERRORTYPE port_audio::set_default_format(size_t index)
{
	if (index < 0 || formats.size() <= index) {
		return OMX_ErrorBadParameter;
	}

	default_format = index;

	return OMX_ErrorNone;
}

} //namespace mf
