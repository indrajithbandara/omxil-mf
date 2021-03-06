﻿#ifndef OMX_MF_PORT_HPP__
#define OMX_MF_PORT_HPP__

#include <vector>
#include <mutex>
#include <condition_variable>
#include <thread>

#include <OMX_Component.h>
#include <OMX_Core.h>

#include <omxil_mf/base.h>
#include <omxil_mf/ring/ring_buffer.hpp>
#include <omxil_mf/ring/bounded_buffer.hpp>
#include <omxil_mf/port_buffer.hpp>
#include <omxil_mf/port_format.hpp>


//OpenMAX バッファを受け渡すバッファの深さ
#define OMX_MF_BUFS_DEPTH    10


namespace mf {

class OMX_MF_API_CLASS port {
public:
	//親クラス
	//typedef xxxx super;
	//ポートバッファをキューイングするリングバッファの型
	typedef ring_buffer<std::vector<port_buffer>::iterator, port_buffer> portbuf_ring_t;
	typedef bounded_buffer<portbuf_ring_t, port_buffer> portbuf_bound_t;

	//disable default constructor
	port() = delete;

	/**
	 * ポートを作成します。
	 *
	 * NOTE:
	 * ポートのインデックス番号は不変です。
	 *
	 * @param ind ポートのインデックス番号
	 * @param c   ポートが所属するコンポーネント
	 */
	port(int ind, component *c);

	virtual ~port();

	//disable copy constructor
	port(const port& obj) = delete;

	//disable operator=
	port& operator=(const port& obj) = delete;

	virtual const char *get_name() const;

	/**
	 * ポートのインデックスを取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nPortIndex に相当します。
	 *
	 * @return ポートのインデックス
	 */
	virtual OMX_U32 get_port_index() const;

	/**
	 * ポートのインデックスを設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nPortIndex に相当します。
	 *
	 * @param v ポートのインデックス
	 */
	virtual void set_port_index(OMX_U32 v);

	/**
	 * ポートの入出力方向を取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::eDir に相当します。
	 *
	 * @return ポートの入出力方向
	 */
	virtual OMX_DIRTYPE get_dir() const;

	/**
	 * ポートの入出力方向を設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::eDir に相当します。
	 *
	 * @param v ポートの入出力方向
	 */
	virtual void set_dir(OMX_DIRTYPE v);

	/**
	 * ポートが持つバッファ数を取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferCountActual に相当します。
	 *
	 * @return ポートが持つバッファ数
	 */
	virtual OMX_U32 get_buffer_count_actual() const;

	/**
	 * ポートが持つバッファ数を設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferCountActual に相当します。
	 *
	 * @param v ポートが持つバッファ数
	 */
	virtual void set_buffer_count_actual(OMX_U32 v);

	/**
	 * ポートが持つ最小のバッファ数を取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferCountMin に相当します。
	 *
	 * @return ポートの最小バッファ数
	 */
	virtual OMX_U32 get_buffer_count_min() const;

	/**
	 * ポートが持つ最小のバッファ数を設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferCountMin に相当します。
	 *
	 * @param v ポートの最小バッファ数
	 */
	virtual void set_buffer_count_min(OMX_U32 v);

	/**
	 * ポートのバッファサイズを取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferSize に相当します。
	 *
	 * @return ポートのバッファサイズ
	 */
	virtual OMX_U32 get_buffer_size() const;

	/**
	 * ポートのバッファサイズを設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferSize に相当します。
	 *
	 * @param v ポートのバッファサイズ
	 */
	virtual void set_buffer_size(OMX_U32 v);

	/**
	 * ポートの有効、無効を取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::bEnabled に相当します。
	 *
	 * @return ポートが有効ならば OMX_TRUE、無効 ならば OMX_FALSE
	 */
	virtual OMX_BOOL get_enabled() const;

	/**
	 * ポートの有効、無効を設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::bEnabled に相当します。
	 *
	 * @param v ポートが有効ならば OMX_TRUE、無効ならば OMX_FALSE
	 */
	virtual void set_enabled(OMX_BOOL v);

	/**
	 * ポートが 'populated' かどうかを取得します。
	 *
	 * 'populated' とはポートが enabled であり、なおかつ、
	 * nBufferCountActual で示す数だけ、
	 * OMX_UseBuffer または OMX_AllocateBuffer にてバッファが確保され、
	 * OMX_StateIdle に遷移する準備ができている状態を指します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::bPopulated に相当します。
	 *
	 * @return ポートが 'populated' ならば OMX_TRUE、
	 * そうでなければ OMX_FALSE
	 */
	virtual OMX_BOOL get_populated() const;

	/**
	 * ポートが 'populated' かどうかを設定します。
	 *
	 * 'populated' とはポートが enabled であり、なおかつ、
	 * nBufferCountActual で示す数だけ、
	 * OMX_UseBuffer または OMX_AllocateBuffer にてバッファが確保され、
	 * OMX_StateIdle に遷移する準備ができている状態を指します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::bPopulated に相当します。
	 *
	 * @param v ポートが 'populated' ならば OMX_TRUE、
	 * そうでなければ OMX_FALSE
	 */
	virtual void set_populated(OMX_BOOL v);

	/**
	 * ポートが 'populated' になるまで待ちます。
	 *
	 * 'populated' とはポートが enabled であり、なおかつ、
	 * nBufferCountActual で示す数だけ、
	 * OMX_UseBuffer または OMX_AllocateBuffer にてバッファが確保され、
	 * OMX_StateIdle に遷移する準備ができている状態を指します。
	 *
	 * <pre>
	 * バッファ確保数        | populated | no buffer | 状態
	 * ----------------------+-----------+-----------+---------------------
	 * 0                     | No        | Yes       | StateLoaded 遷移可能
	 * < nBufferCountActual  | No        | No        |
	 * >= nBufferCountActual | Yes       | No        | StateIdle 遷移可能
	 * ----------------------+-----------+-----------+---------------------
	 * </pre>
	 *
	 * @param v 待ちたい状態
	 * 	OMX_TRUE なら populated になるまで待ち、
	 * 	OMX_FALSE なら populated ではなくなるまで待ちます。
	 */
	virtual void wait_populated(OMX_BOOL v);

	/**
	 * ポートの種別を取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::eDomain に相当します。
	 *
	 * @return ポートの種別
	 */
	virtual OMX_PORTDOMAINTYPE get_domain() const;

	/**
	 * ポートの種別を設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::eDomain に相当します。
	 *
	 * @param v ポートの種別
	 */
	virtual void set_domain(OMX_PORTDOMAINTYPE v);

	/**
	 * バッファが物理アドレス上連続でなければならないかどうかを取得します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::bBuffersContiguous に相当します。
	 *
	 * @return 物理アドレス上連続でなければならないならば OMX_TRUE、
	 * そうでなければ OMX_FALSE
	 */
	virtual OMX_BOOL get_buffers_contiguous() const;

	/**
	 * バッファが物理アドレス上連続でなければならないかどうかを設定します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::bBuffersContiguous に相当します。
	 *
	 * @param v 物理アドレス上連続でなければならないならば OMX_TRUE、
	 * そうでなければ OMX_FALSE
	 */
	virtual void set_buffers_contiguous(OMX_BOOL v);

	/**
	 * バッファをアラインメントすべきアドレスを取得します。
	 *
	 * 例えば 4 ならば、バッファは 4バイト境界に
	 * アラインメントされていなければなりません。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferAlignment に相当します。
	 *
	 * @return アラインメントすべきアドレス、0 ならばアラインメント不要
	 */
	virtual OMX_U32 get_buffer_alignment() const;

	/**
	 * バッファをアラインメントすべきアドレスを設定します。
	 *
	 * 例えば 4 ならば、バッファは 4バイト境界に
	 * アラインメントされていなければなりません。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE::nBufferAlignment に相当します。
	 *
	 * @param v アラインメントすべきアドレス、0 ならばアラインメント不要
	 */
	virtual void set_buffer_alignment(OMX_U32 v);


	/**
	 * ポートが所属するコンポーネントを取得します。
	 *
	 * @return コンポーネントへのポインタ、属していなければ nullptr
	 */
	virtual const component *get_component() const;

	/**
	 * ポートが所属するコンポーネントを取得します。
	 *
	 * @return コンポーネントへのポインタ、属していなければ nullptr
	 */
	virtual component *get_component();

	/**
	 * ポートからの読み出し、または書き込みを禁止し、
	 * 全ての待機しているスレッドを強制的に解除（シャットダウン）します。
	 *
	 * 強制解除されたスレッドは runtime_error をスローします。
	 *
	 * @param rd 以降の読み出しを禁止し、
	 * 	読み出しの待機状態を解除する場合は true、
	 * 	変更しない場合は false を指定します
	 * @param wr 以降の書き込みを禁止し、
	 * 	書き込みの待機状態を解除する場合は true、
	 * 	変更しない場合は false を指定します
	 */
	virtual void shutdown(bool rd, bool wr);

	/**
	 * シャットダウン処理を中止し、
	 * ポートからの読み出し、または書き込みを許可します。
	 *
	 * FIXME: シャットダウン処理の中止後、
	 * ポートがシャットダウン前と同様に動作するかどうかは、
	 * 各ポートの実装に依存します。
	 *
	 * @param rd 以降の読み出しを許可する場合は true、
	 * 	変更しない場合は false を指定します
	 * @param wr 以降の書き込みを許可する場合は true、
	 * 	変更しない場合は false を指定します
	 */
	virtual void abort_shutdown(bool rd, bool wr);

	/**
	 * 読み出し側のシャットダウン処理中かどうかを取得します。
	 *
	 * @return 処理中ならば true、そうでなければ false
	 */
	virtual bool is_shutting_read() const;

	/**
	 * 書き込み側のシャットダウン処理中かどうかを取得します。
	 *
	 * @return 処理中ならば true、そうでなければ false
	 */
	virtual bool is_shutting_write() const;

	/**
	 * ポートのバッファが全て解放されているかどうか取得します。
	 *
	 * 注: 'no buffer' はこのライブラリ独自の用語です。
	 * OpenMAX IL の用語ではありません。
	 *
	 * 'no buffer' とはポートが enabled であり、なおかつ、
	 * OMX_FreeBuffer にて全てのバッファが解放され、
	 * OMX_StateLoaded に遷移する準備ができている状態を指します。
	 *
	 * @return バッファが全て解放されていれば OMX_TRUE、
	 * そうでなければ OMX_FALSE
	 */
	virtual OMX_BOOL get_no_buffer() const;

	/**
	 * ポートのバッファが全て解放されているかどうか設定します。
	 *
	 * 注: 'no buffer' はこのライブラリ独自の用語です。
	 * OpenMAX IL の用語ではありません。
	 *
	 * 'no buffer' とはポートが enabled であり、なおかつ、
	 * OMX_FreeBuffer にて全てのバッファが解放され、
	 * OMX_StateLoaded に遷移する準備ができている状態を指します。
	 *
	 * @param v バッファが全て解放されていれば OMX_TRUE、
	 * そうでなければ OMX_FALSE
	 */
	virtual void set_no_buffer(OMX_BOOL v);

	/**
	 * ポートのバッファが全て解放されるまで待ちます。
	 *
	 * 注: 'no buffer' はこのライブラリ独自の用語です。
	 * OpenMAX IL の用語ではありません。
	 *
	 * 'no buffer' とはポートが enabled であり、なおかつ、
	 * OMX_FreeBuffer にて全てのバッファが解放され、
	 * OMX_StateLoaded に遷移する準備ができている状態を指します。
	 *
	 * <pre>
	 * バッファ確保数        | populated | no buffer | 状態
	 * ----------------------+-----------+-----------+---------------------
	 * 0                     | No        | Yes       | StateLoaded 遷移可能
	 * < nBufferCountActual  | No        | No        |
	 * >= nBufferCountActual | Yes       | No        | StateIdle 遷移可能
	 * ----------------------+-----------+-----------+---------------------
	 * </pre>
	 *
	 * @param v 待ちたい状態
	 * 	OMX_TRUE なら no buffer になるまで待ち、
	 * 	OMX_FALSE なら no buffer ではなくなるまで待ちます。
	 */
	virtual void wait_no_buffer(OMX_BOOL v) const;

	/**
	 * ポートの 'populated', 'no buffer' 状態を更新します。
	 *
	 * 注: 'no buffer' はこのライブラリ独自の用語です。
	 * OpenMAX IL の用語ではありません。
	 *
	 * 'populated' とはポートが enabled であり、なおかつ、
	 * nBufferCountActual で示す数だけ、
	 * OMX_UseBuffer または OMX_AllocateBuffer にてバッファが確保され、
	 * OMX_StateIdle に遷移する準備ができている状態を指します。
	 *
	 * 'no buffer' とはポートが enabled であり、なおかつ、
	 * OMX_FreeBuffer にて全てのバッファが解放され、
	 * OMX_StateLoaded に遷移する準備ができている状態を指します。
	 *
	 * <pre>
	 * バッファ確保数        | populated | no buffer | 状態
	 * ----------------------+-----------+-----------+---------------------
	 * 0                     | No        | Yes       | StateLoaded 遷移可能
	 * < nBufferCountActual  | No        | No        |
	 * >= nBufferCountActual | Yes       | No        | StateIdle 遷移可能
	 * ----------------------+-----------+-----------+---------------------
	 * </pre>
	 */
	virtual void update_buffer_status();

	/**
	 * Get OpenMAX IL definition data of this port.
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE の各メンバに設定値がセットされます。
	 * ただし format メンバは 0 で埋められます。
	 *
	 * <pre>
	 * struct OMX_PARAM_PORTDEFINITIONTYPE {
	 *     OMX_U32 nSize;
	 *     OMX_VERSIONTYPE nVersion;
	 *     OMX_U32 nPortIndex;
	 *     OMX_DIRTYPE eDir;
	 *     OMX_U32 nBufferCountActual;
	 *     OMX_U32 nBufferCountMin;
	 *     OMX_U32 nBufferSize;
	 *     OMX_BOOL bEnabled;
	 *     OMX_BOOL bPopulated;
	 *     OMX_PORTDOMAINTYPE eDomain;
	 *     union {
	 *         OMX_AUDIO_PORTDEFINITIONTYPE audio;
	 *         OMX_VIDEO_PORTDEFINITIONTYPE video;
	 *         OMX_IMAGE_PORTDEFINITIONTYPE image;
	 *         OMX_OTHER_PORTDEFINITIONTYPE other;
	 *     } format;
	 *     OMX_BOOL bBuffersContiguous;
	 *     OMX_U32 nBufferAlignment;
	 * }
	 * </pre>
	 *
	 * @return port definition data of OpenMAX IL
	 */
	virtual const OMX_PARAM_PORTDEFINITIONTYPE *get_definition() const;

	/**
	 * Set OpenMAX IL definition data of this port.
	 *
	 * コンポーネントや、ポート自身が definition data を設定する場合、
	 * こちらのメンバ関数を呼び出します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE の各メンバに対応して、
	 * このポートのメンバ変数が更新されます。
	 * ただし nSize, nVersion, format メンバは無視されます。
	 *
	 * <pre>
	 * struct OMX_PARAM_PORTDEFINITIONTYPE {
	 *     OMX_U32 nSize;
	 *     OMX_VERSIONTYPE nVersion;
	 *     OMX_U32 nPortIndex;
	 *     OMX_DIRTYPE eDir;
	 *     OMX_U32 nBufferCountActual;
	 *     OMX_U32 nBufferCountMin;
	 *     OMX_U32 nBufferSize;
	 *     OMX_BOOL bEnabled;
	 *     OMX_BOOL bPopulated;
	 *     OMX_PORTDOMAINTYPE eDomain;
	 *     union {
	 *         OMX_AUDIO_PORTDEFINITIONTYPE audio;
	 *         OMX_VIDEO_PORTDEFINITIONTYPE video;
	 *         OMX_IMAGE_PORTDEFINITIONTYPE image;
	 *         OMX_OTHER_PORTDEFINITIONTYPE other;
	 *     } format;
	 *     OMX_BOOL bBuffersContiguous;
	 *     OMX_U32 nBufferAlignment;
	 * }
	 * </pre>
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE set_definition(const OMX_PARAM_PORTDEFINITIONTYPE& v);

	/**
	 * Set OpenMAX IL definition data of this port by IL Client.
	 *
	 * OpenMAX IL クライアントから definition data を設定された場合、
	 * こちらのメンバ関数を呼び出します。
	 *
	 * OMX_PARAM_PORTDEFINITIONTYPE の各メンバに対応した、
	 * このポートのメンバ変数が更新されます。
	 * ただし nBufferCountActual メンバ以外は全て無視されます。
	 *
	 * <pre>
	 * struct OMX_PARAM_PORTDEFINITIONTYPE {
	 *     OMX_U32 nSize;
	 *     OMX_VERSIONTYPE nVersion;
	 *     OMX_U32 nPortIndex;
	 *     OMX_DIRTYPE eDir;
	 *     OMX_U32 nBufferCountActual;
	 *     OMX_U32 nBufferCountMin;
	 *     OMX_U32 nBufferSize;
	 *     OMX_BOOL bEnabled;
	 *     OMX_BOOL bPopulated;
	 *     OMX_PORTDOMAINTYPE eDomain;
	 *     union {
	 *         OMX_AUDIO_PORTDEFINITIONTYPE audio;
	 *         OMX_VIDEO_PORTDEFINITIONTYPE video;
	 *         OMX_IMAGE_PORTDEFINITIONTYPE image;
	 *         OMX_OTHER_PORTDEFINITIONTYPE other;
	 *     } format;
	 *     OMX_BOOL bBuffersContiguous;
	 *     OMX_U32 nBufferAlignment;
	 * }
	 * </pre>
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE set_definition_from_client(const OMX_PARAM_PORTDEFINITIONTYPE& v);

	virtual OMX_BOOL get_tunneled() const;
	virtual void set_tunneled(OMX_BOOL v);

	virtual OMX_HANDLETYPE get_tunneled_component() const;
	virtual void set_tunneled_component(OMX_HANDLETYPE v);

	virtual OMX_U32 get_tunneled_port() const;
	virtual void set_tunneled_port(OMX_U32 v);

	virtual OMX_BOOL get_tunneled_supplier() const;
	virtual void set_tunneled_supplier(OMX_BOOL v);

	/**
	 * ポートがサポートするデータ形式を追加します。
	 *
	 * @param f データ形式へのポインタ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE add_port_format(const port_format& f);

	/**
	 * ポートがサポートするオーディオデータ形式を追加します。
	 *
	 * @param f データ形式への参照
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE add_port_format(const OMX_AUDIO_PARAM_PORTFORMATTYPE& f);

	/**
	 * ポートがサポートするビデオデータ形式を追加します。
	 *
	 * @param f データ形式への参照
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE add_port_format(const OMX_VIDEO_PARAM_PORTFORMATTYPE& f);

	/**
	 * ポートがサポートする画像データ形式を追加します。
	 *
	 * @param f データ形式への参照
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE add_port_format(const OMX_IMAGE_PARAM_PORTFORMATTYPE& f);

	/**
	 * ポートがサポートするその他のデータ形式を追加します。
	 *
	 * @param f データ形式への参照
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE add_port_format(const OMX_OTHER_PARAM_PORTFORMATTYPE& f);

	/**
	 * ポートがサポートするデータの形式を削除します。
	 *
	 * @param index データ形式のインデックス
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE remove_port_format(size_t index);

	/**
	 * ポートがサポートするデータの形式を取得します。
	 *
	 * 取得した port_format は Audio/Video/Image/Other いずれかの、
	 * OMX_XXXXX_PARAM_PORTFORMATTYPE を保持しています。
	 *
	 * 下記のように必要な種類の PORTFORMATTYPE を取得してください。
	 *
	 * <pre>
	 * const port_format *pf = get_port_format(index);
	 * if (pf == nullptr) {
	 *     //Invalid index
	 * }
	 * //If you need audio port format type
	 * const OMX_AUDIO_PARAM_PORTFORMATTYPE *pf_audio = pf->get_audio();
	 * if (pf_audio == nullptr) {
	 *     //Not audio
	 * }
	 * </pre>
	 *
	 * @param index データ形式のインデックス
	 * @return データ形式へのポインタ、取得できなければ nullptr
	 */
	virtual const port_format *get_port_format(size_t index) const;

	/**
	 * 条件を指定してポートがサポートするデータの形式を取得します。
	 *
	 * 指定した条件が複数のデータ形式に該当する場合、
	 * index の値が小さいデータ形式が優先されます。
	 *
	 * 参考: OpenMAX IL 1.2.0 Specification:
	 *   4.1.7 OMX_AUDIO_PARAM_PORTFORMATTYPE,
	 *   4.3.5 OMX_VIDEO_PARAM_PORTFORMATTYPE
	 *
	 * 下記のように設定したい種類の PORTFORMATTYPE を渡して、
	 * port_format を作成してください。
	 *
	 * <pre>
	 * OMX_VIDEO_PARAM_PORTFORMATTYPE fmt;
	 * fmt.eCompressionFormat = OMX_VIDEO_CodingMPEG2;
	 * fmt.eColorFormat       = OMX_COLOR_FormatUnused; //don't care
	 * fmt.xFramerate         = 0; //don't care
	 *
	 * const port_format *pfmt = get_port_format_index(port_format(pfmt));
	 * </pre>
	 *
	 * @param f データ形式
	 * @return データ形式へのポインタ、取得できなければ nullptr
	 */
	virtual const port_format *get_port_format(const port_format& f) const;

	/**
	 * ポートがサポートするデータの形式のインデクスを取得します。
	 *
	 * 指定した条件が複数のデータ形式に該当する場合、
	 * index の値が小さいデータ形式が優先されます。
	 *
	 * 参考: OpenMAX IL 1.2.0 Specification:
	 *   4.1.7 OMX_AUDIO_PARAM_PORTFORMATTYPE,
	 *   4.3.5 OMX_VIDEO_PARAM_PORTFORMATTYPE
	 *
	 * 下記のように設定したい種類の PORTFORMATTYPE を渡して、
	 * port_format を作成してください。
	 *
	 * <pre>
	 * OMX_VIDEO_PARAM_PORTFORMATTYPE fmt;
	 * fmt.eCompressionFormat = OMX_VIDEO_CodingMPEG2;
	 * fmt.eColorFormat       = OMX_COLOR_FormatUnused; //don't care
	 * fmt.xFramerate         = 0; //don't care
	 *
	 * size_t index;
	 * get_port_format_index(port_format(pfmt), &index);
	 * </pre>
	 *
	 * NOTE:
	 * このクラスの実装では、常に成功し、インデクスは (~0) を返します。
	 * 継承したクラスで適切にオーバライドしてください。
	 *
	 * @param f   データ形式
	 * @param ind データ形式のインデクス、取得できなければ (~0)
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE get_port_format_index(const port_format& f, size_t *ind) const;

	/**
	 * ポートがデフォルトでサポートするデータの形式を取得します。
	 *
	 * デフォルトに指定したフォーマットの index を
	 * default_format_index と置いたとき、
	 * 下記の呼び出しに等しいです。
	 *
	 * <pre>
	 * get_port_format(default_format_index);
	 * </pre>
	 *
	 * @return データ形式へのポインタ
	 */
	virtual const port_format *get_default_format() const;

	/**
	 * ポートがデフォルトでサポートするデータの形式を設定します。
	 *
	 * @param index データ形式のインデックス
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE set_default_format(size_t index);

	/**
	 * ポートがデフォルトでサポートするデータの形式を設定します。
	 *
	 * port_format は Audio/Video/Image/Other いずれかの、
	 * OMX_XXXXX_PARAM_PORTFORMATTYPE を保持している必要があります。
	 *
	 * 下記のように設定したい種類の PORTFORMATTYPE を渡して、
	 * port_format を作成してください。
	 *
	 * <pre>
	 * OMX_XXXXX_PARAM_PORTFORMATTYPE fmt;
	 * fmt.eEncoding = OMX_AUDIO_CodingPCM;
	 *
	 * port_format pfmt = port_format(fmt);
	 * port->set_port_format(&pfmt);
	 * </pre>
	 *
	 * @param f ポートのデータ形式
	 * @return OpenMAX エラー値
	 *     OMX_ErrorBadParameter      : データ形式に何も指定していない、
	 *                                  もしくは矛盾している
	 *     OMX_ErrorUnsupportedSetting: 対応していないデータ形式を指定した
	 */
	virtual OMX_ERRORTYPE set_default_format(const port_format& f);

	/**
	 * ポートを無効にします。
	 *
	 * empty_buffer または fill_buffer によって渡されているが、
	 * 未処理のバッファは全て返却されます。
	 *
	 * 確保していたバッファを全て解放します。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE disable_port();

	/**
	 * ポートを有効にします。
	 *
	 * use_buffer または allocate_buffer によって、
	 * 確保すべきバッファが全て確保されるまで、待機します。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE enable_port();

	/**
	 * IL クライアントからポートへのバッファ処理要求を禁止します。
	 *
	 * このメソッドを呼び出した後
	 * クライアントからの EmptyThisBuffer と FillThisBuffer の呼び出しは、
	 * 全てエラーとなります。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE plug_client_request();

	/**
	 * IL クライアントからポートへのバッファ処理要求を許可します。
	 *
	 * このメソッドを呼び出した後
	 * EmptyThisBuffer と FillThisBuffer の呼び出しは、
	 * 正常に行われます。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE unplug_client_request();

	/**
	 * コンポーネントからポートへのバッファ処理要求を禁止します。
	 *
	 * このメソッドを呼び出した後
	 * pop_buffer メソッドの呼び出しは、
	 * 全てエラーとなります。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE plug_component_request();

	/**
	 * コンポーネントからポートへのバッファ処理要求を許可します。
	 *
	 * このメソッドを呼び出した後
	 * pop_buffer メソッドの呼び出しは、
	 * 正常に行われます。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE unplug_component_request();

	/**
	 * 未処理のバッファを強制的に全て返却します。
	 *
	 * empty_buffer または fill_buffer によって渡されているが、
	 * コンポーネントが処理していないバッファを全て返却します。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE return_buffers_force();

	/**
	 * 全てのバッファを EmptyBufferDone あるいは FillBufferDone にて、
	 * 返却するまで待ちます。
	 */
	virtual void wait_buffer_returned() const;

	/**
	 * 指定されたコンポーネントのポートと、トンネル接続します。
	 *
	 * @param omx_comp OpenMAX IL コンポーネント、
	 *                 トンネル接続を解除する場合は nullptr
	 * @param index    コンポーネントのポート番号
	 * @param setup    トンネル接続の設定
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE component_tunnel_request(OMX_HANDLETYPE omx_comp, OMX_U32 index, OMX_TUNNELSETUPTYPE *setup);

	/**
	 * トンネル接続先のポート用のバッファを確保し、
	 * トンネル接続先のポートに OMX_UseBuffer で渡します。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE allocate_tunnel_buffers();

	/**
	 * トンネル接続先のポートに OMX_FreeBuffer でバッファを解放させ、
	 * 確保していたバッファを削除します。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE free_tunnel_buffers();

	/**
	 * コンポーネント外部で確保されたバッファを、
	 * コンポーネントとのデータのやり取りに使用します。
	 *
	 * <pre>
	 * struct OMX_BUFFERHEADERTYPE {
	 *     OMX_U32 nSize;
	 *     OMX_VERSIONTYPE nVersion;
	 *     OMX_U8* pBuffer;
	 *     OMX_U32 nAllocLen;
	 *     OMX_U32 nFilledLen;
	 *     OMX_U32 nOffset;
	 *     OMX_PTR pAppPrivate;
	 *     OMX_PTR pPlatformPrivate;
	 *     OMX_PTR pInputPortPrivate;
	 *     OMX_PTR pOutputPortPrivate;
	 *     OMX_HANDLETYPE hMarkTargetComponent;
	 *     OMX_PTR pMarkData;
	 *     OMX_U32 nTickCount;
	 *     OMX_TICKS nTimeStamp;
	 *     OMX_U32 nFlags;
	 *     OMX_U32 nOutputPortIndex;
	 *     OMX_U32 nInputPortIndex;
	 * }
	 * </pre>
	 *
	 * @param bufhead OpenMAX バッファヘッダを受け取るためのポインタ
	 * @param priv    アプリケーション（pAppPrivate）で使うデータのポインタ
	 * @param size    バッファサイズ
	 * @param buf     バッファ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE use_buffer(OMX_BUFFERHEADERTYPE **bufhead, OMX_PTR priv, OMX_U32 size, OMX_U8 *buf);

	/**
	 * コンポーネントでバッファを確保し、
	 * コンポーネントとのデータのやり取りに使用します。
	 *
	 * <pre>
	 * struct OMX_BUFFERHEADERTYPE {
	 *     OMX_U32 nSize;
	 *     OMX_VERSIONTYPE nVersion;
	 *     OMX_U8* pBuffer;
	 *     OMX_U32 nAllocLen;
	 *     OMX_U32 nFilledLen;
	 *     OMX_U32 nOffset;
	 *     OMX_PTR pAppPrivate;
	 *     OMX_PTR pPlatformPrivate;
	 *     OMX_PTR pInputPortPrivate;
	 *     OMX_PTR pOutputPortPrivate;
	 *     OMX_HANDLETYPE hMarkTargetComponent;
	 *     OMX_PTR pMarkData;
	 *     OMX_U32 nTickCount;
	 *     OMX_TICKS nTimeStamp;
	 *     OMX_U32 nFlags;
	 *     OMX_U32 nOutputPortIndex;
	 *     OMX_U32 nInputPortIndex;
	 * }
	 * </pre>
	 *
	 * @param bufhead OpenMAX バッファヘッダを受け取るためのポインタ
	 * @param priv    アプリケーション（pAppPrivate）で使うデータのポインタ
	 * @param size    確保するバッファサイズ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE allocate_buffer(OMX_BUFFERHEADERTYPE **bufhead, OMX_PTR priv, OMX_U32 size);

	/**
	 * バッファを解放します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE free_buffer(OMX_BUFFERHEADERTYPE *bufhead);

	/**
	 * バッファを解放します。
	 *
	 * @param pb ポートバッファ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE free_buffer(port_buffer *pb);

	/**
	 * バッファを検索します。
	 *
	 * 指定したバッファヘッダが保持している pBuffer が、
	 * このポートに登録されているかどうかを調べます。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return 指定したバッファが見つかれば true、見つからなければ false
	 */
	virtual bool find_buffer(const OMX_BUFFERHEADERTYPE *bufhead) const;

	/**
	 * バッファを検索します。
	 *
	 * 指定したポートバッファの header->pBuffer が、
	 * このポートに登録されているかどうかを調べます。
	 *
	 * @param pb ポートバッファ
	 * @return 指定したバッファが見つかれば true、見つからなければ false
	 */
	virtual bool find_buffer(const port_buffer *pb) const;

	/**
	 * クライアントから受け取ったが、
	 * クライアントに返していないバッファをリストに追加します。
	 *
	 * @param pb ポートバッファ
	 * @return OpenMAX エラー値
	 */
	OMX_ERRORTYPE add_held_buffer(const port_buffer *pb);

	/**
	 * クライアントから受け取ったが、
	 * クライアントに返していないバッファをリストから削除します。
	 *
	 * @param pb ポートバッファ
	 * @return OpenMAX エラー値
	 */
	OMX_ERRORTYPE remove_held_buffer(const port_buffer *pb);

	//----------------------------------------
	// コンポーネント利用者 → コンポーネントへのバッファ送付
	//----------------------------------------

	/**
	 * ポートとポートが所属しているコンポーネントに対し、
	 * 指定した OpenMAX バッファからデータを読み出すことを要求します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE empty_buffer(OMX_BUFFERHEADERTYPE *bufhead);

	/**
	 * ポートとポートが所属しているコンポーネントに対し、
	 * 指定した OpenMAX バッファにデータを書き込むことを要求します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE fill_buffer(OMX_BUFFERHEADERTYPE *bufhead);

	/**
	 * OpenMAX バッファを受け付け、バッファ処理スレッドに送出します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE push_buffer(OMX_BUFFERHEADERTYPE *bufhead);


	/**
	 * 受け付けた OpenMAX バッファを持つポートバッファを引き出します。
	 *
	 * バッファ処理スレッドにて使用します。
	 *
	 * @param pb ポートバッファ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE pop_buffer(port_buffer *pb);


	//----------------------------------------
	// コンポーネント → コンポーネント利用者へのバッファ返却
	//----------------------------------------

	/**
	 * ポートとポートが所属しているコンポーネントに対し、
	 * データを読み出し終わった OpenMAX バッファの返却を要求します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE empty_buffer_done(OMX_BUFFERHEADERTYPE *bufhead);

	/**
	 * ポートとポートが所属しているコンポーネントに対し、
	 * データを読み出し終わったポートバッファの返却を要求します。
	 *
	 * @param pb ポートバッファ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE empty_buffer_done(port_buffer *pb);

	/**
	 * ポートとポートが所属しているコンポーネントに対し、
	 * データを書き込み終わった OpenMAX バッファの返却を要求します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE fill_buffer_done(OMX_BUFFERHEADERTYPE *bufhead);

	/**
	 * ポートとポートが所属しているコンポーネントに対し、
	 * データを書き込み終わったポートバッファの返却を要求します。
	 *
	 * @param pb ポートバッファ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE fill_buffer_done(port_buffer *pb);

	/**
	 * 使用後の OpenMAX バッファを、バッファ返却スレッドに送出します。
	 *
	 * @param bufhead OpenMAX バッファヘッダ
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE push_buffer_done(OMX_BUFFERHEADERTYPE *bufhead);

	/**
	 * トンネル接続されたポート間の転送を開始します。
	 *
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE start_tunneling();


protected:
	/**
	 * ポートが対応しているフォーマットのリストを取得します。
	 *
	 * 継承先のクラスにて、
	 * フォーマットのリストを直接参照したいときに用います。
	 *
	 * @return フォーマットのリストへの参照
	 */
	virtual const std::vector<port_format>& get_port_format_list() const;

	/**
	 * ポートの待ちを強制キャンセルすべきか、
	 * そうでないかをチェックし、キャンセルすべきなら例外をスローします。
	 *
	 * @param lock ポートのロック
	 */
	virtual void error_if_broken(std::unique_lock<std::recursive_mutex>& lock) const;

	/**
	 * ポートの待ちを強制キャンセルすべきかを取得します。
	 *
	 * shutdown_read または shutdown_write により、
	 * 強制キャンセルが発生します。
	 *
	 * @return 強制キャンセルすべきであれば true、そうでなければ false
	 */
	virtual bool is_broken() const;

	/**
	 * 送出、返却した OpenMAX バッファ数を更新し、通知します。
	 */
	virtual void notify_buffer_count();

	/**
	 * 指定されたコンポーネントのポートと、トンネル接続します。
	 * （入力ポート用）
	 *
	 * NOTE: この関数は引数のエラーチェックをしません。
	 *
	 * @param omx_comp OpenMAX IL コンポーネント、
	 *                 トンネル接続を解除する場合は nullptr
	 * @param index    コンポーネントのポート番号
	 * @param setup    トンネル接続の設定
	 * @param def      ポートの定義情報
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE component_tunnel_request_input(OMX_HANDLETYPE omx_comp, OMX_U32 index, OMX_TUNNELSETUPTYPE *setup, OMX_PARAM_PORTDEFINITIONTYPE *def);

	/**
	 * 指定されたコンポーネントのポートと、トンネル接続します。
	 * （出力ポート用）
	 *
	 * NOTE: この関数は引数のエラーチェックをしません。
	 *
	 * @param omx_comp OpenMAX IL コンポーネント、
	 *                 トンネル接続を解除する場合は nullptr
	 * @param index    コンポーネントのポート番号
	 * @param setup    トンネル接続の設定
	 * @param def      ポートの定義情報
	 * @return OpenMAX エラー値
	 */
	virtual OMX_ERRORTYPE component_tunnel_request_output(OMX_HANDLETYPE omx_comp, OMX_U32 index, OMX_TUNNELSETUPTYPE *setup, OMX_PARAM_PORTDEFINITIONTYPE *def);

	//----------------------------------------
	// コンポーネント利用者へのバッファ返却スレッド
	//----------------------------------------

	/**
	 * 使用後の OpenMAX バッファを返却します。
	 *
	 * 使用後の OpenMAX バッファを受け取り EmptyThisBufferDone または
	 * FillThisBufferDone コールバックにて、バッファの供給元に返します。
	 *
	 * @return 常に nullptr
	 */
	virtual void *buffer_done();

	/**
	 * 使用後の OpenMAX バッファを返却するスレッドの main 関数です。
	 *
	 * @param p ポートへのポインタ
	 * @return 常に nullptr
	 */
	static void *buffer_done_thread_main(port *p);


protected:
	//temporary buffer for get_definition() member function.
	mutable OMX_PARAM_PORTDEFINITIONTYPE definition;


private:
	//ポートのロック
	mutable std::recursive_mutex mut;
	//ポートの状態変数
	mutable std::condition_variable_any cond;

	//以下 OMX_PARAM_PORTDEFINITIONTYPE に基づくメンバ

	OMX_U32 port_index;
	OMX_DIRTYPE dir;
	OMX_U32 buffer_count_actual;
	OMX_U32 buffer_count_min;
	OMX_U32 buffer_size;
	OMX_BOOL f_enabled;
	OMX_BOOL f_populated;
	OMX_PORTDOMAINTYPE domain;
	OMX_BOOL buffers_contiguous;
	OMX_U32 buffer_alignment;

	//以上 OMX_PARAM_PORTDEFINITIONTYPE に基づくメンバ


	//ポートが所属するコンポーネント
	component *comp;

	//待機の強制解除フラグ
	bool shutting_read, shutting_write;

	//ポートに使用可能なバッファが一つもないことを示すフラグ
	OMX_BOOL f_no_buffer;

	//ポートがトンネルモードかどうか
	OMX_BOOL f_tunneled;
	//トンネル接続先のポートが所属するコンポーネント
	OMX_HANDLETYPE tunneled_comp;
	//トンネル接続先のポート
	OMX_U32 tunneled_port;
	//バッファ供給側（Supplier）か、使用側（User）か
	OMX_BOOL f_tunneled_supplier;

	//ポートがサポートするフォーマットのリスト
	//OMX_GetParameter(OMX_IndexParamXxxxxPortFormat) にて使用します。
	//Xxxxx は Audio | Video | Image | Other のいずれかです。
	std::vector<port_format> formats;
	int default_format;

	//使用可能バッファ登録リスト
	std::vector<port_buffer *> list_bufs;
	//クライアントから受け取ったが、未返却のバッファのリスト
	//ポートのフラッシュ時にリスト内のバッファを強制的に返却します。
	std::vector<port_buffer> list_held_bufs;
	mutable std::recursive_mutex mut_list_bufs;

	//バッファ送出用リングバッファ
	std::vector<port_buffer> vec_send;
	portbuf_ring_t *ring_send;
	portbuf_bound_t *bound_send;

	//使用後のバッファ返却用リングバッファ
	std::vector<port_buffer> vec_ret;
	portbuf_ring_t *ring_ret;
	portbuf_bound_t *bound_ret;
	//使用後のバッファ返却スレッド
	std::thread *th_ret;

	//バッファ送出数、返却数のメモ
	uint64_t cnt_send_wr, cnt_recv_rd;
};

} //namespace mf

#endif //OMX_MF_PORT_HPP__
