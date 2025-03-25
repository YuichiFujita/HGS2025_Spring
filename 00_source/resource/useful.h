//============================================================
//
//	便利関数ヘッダー [useful.h]
//	Author：藤田勇一
//
//============================================================
//************************************************************
//	二重インクルード防止
//************************************************************
#ifndef _USEFUL_H_
#define _USEFUL_H_

//************************************************************
//	マクロ定義
//************************************************************
// 便利マクロ
#define SCREEN_WIDTH	(1280)	// ウインドウの横幅
#define SCREEN_HEIGHT	(720)	// ウインドウの縦幅
#define NONE_IDX		(-1)	// インデックス非使用
#define NONE_STRING		("\0")	// 文字列非使用
#define SIZE_TILE		(60)	// マップタイルの大きさ

#define HALF_PI	(D3DX_PI * 0.5f)	// 二分の一の円周率 (π/２)
#define QRTR_PI	(D3DX_PI * 0.25f)	// 四分の一の円周率 (π/４)
#define NUM_ARRAY(a)	(sizeof((a)) / sizeof((a)[0]))	// 配列の要素数計算

// メモリ開放マクロ
#define SAFE_UNINIT(p)		if ((p) != nullptr) { (p)->Uninit();		(p) = nullptr; }	// Uninit関数の破棄マクロ
#define SAFE_FREE(p)		if ((p) != nullptr) { free((p));			(p) = nullptr; }	// free関数の破棄マクロ
#define SAFE_RELEASE(p)		if ((p) != nullptr) { (p)->Release();		(p) = nullptr; }	// Release関数の破棄マクロ
#define SAFE_DEL_OBJECT(p)	if ((p) != nullptr) { DeleteObject((p));	(p) = nullptr; }	// DeleteObject関数の破棄マクロ
#define SAFE_REF_RELEASE(p)	if ((p) != nullptr) { (p)->Release((p)); }						// 参照ポインタ付きRelease関数の破棄マクロ
#define SAFE_DELETE(p)		if ((p) != nullptr) { delete	(p);		(p) = nullptr; }	// 配列を使用しないdeleteを使用する破棄マクロ
#define SAFE_DEL_ARRAY(p)	if ((p) != nullptr) { delete[]	(p);		(p) = nullptr; }	// 配列を使用したdelete[]を使用する破棄マクロ

// マネージャー関係
#define GET_MANAGER			(CManager::GetInstance())					// マネージャーインスタンス取得
#define GET_INPUTKEY		(CManager::GetInstance()->GetKeyboard())	// キーボード情報取得
#define GET_INPUTMOUSE		(CManager::GetInstance()->GetMouse())		// マウス情報取得
#define GET_INPUTPAD		(CManager::GetInstance()->GetPad())			// パッド情報取得
#define GET_RETENTION		(CManager::GetInstance()->GetRetention())	// データ保存情報取得
#define GET_RENDERER		(CManager::GetInstance()->GetRenderer())	// レンダラー情報取得
#define GET_DEVICE			(CManager::GetInstance()->GetRenderer()->GetDevice())					// デバイス情報取得
#define PLAY_SOUND(label)	(CManager::GetInstance()->GetSound()->Play((CSound::ELabel)(label)))	// サウンド再生

// VECTOR2関係
#define VEC2_ZERO	(VECTOR2())		// 0クリア
#define VEC2_ONE	(VECTOR2(1.0f))	// 1クリア

// VECTOR3関係
#define VEC3_ZERO	(VECTOR3())		// 0クリア
#define VEC3_ONE	(VECTOR3(1.0f))	// 1クリア
#define SCREEN_CENT	(VECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f))	// ウインドウの中央座標
#define SCREEN_SIZE	(VECTOR3((float)SCREEN_WIDTH, (float)SCREEN_HEIGHT, 0.0f))	// ウインドウの画面サイズ

// POSGRID2関係
#define GRID2_ZERO	(POSGRID2())	// 0クリア
#define GRID2_ONE	(POSGRID2(1))	// 1クリア

// POSGRID3関係
#define GRID3_ZERO	(POSGRID3())	// 0クリア
#define GRID3_ONE	(POSGRID3(1))	// 1クリア

// MATRIX関係
#define MTX_IDENT	(MATRIX())	// 単位マトリックスクリア

//************************************************************
//	列挙型定義
//************************************************************
// 横配置列挙
enum EAlignX
{
	XALIGN_LEFT = 0,	// 左揃え
	XALIGN_CENTER,		// 中央揃え
	XALIGN_RIGHT,		// 右揃え
	XALIGN_MAX,			// この列挙型の総数
};

// 縦配置列挙
enum EAlignY
{
	YALIGN_TOP = 0,	// 上揃え
	YALIGN_CENTER,	// 中央揃え
	YALIGN_BOTTOM,	// 下揃え
	YALIGN_MAX,		// この列挙型の総数
};

//************************************************************
//	構造体定義
//************************************************************
// 2次元ベクトル
struct VECTOR2 : public D3DXVECTOR2
{
	// コンストラクタ継承
	using D3DXVECTOR2::D3DXVECTOR2;

	// 演算子オーバーライド再公開
	using D3DXVECTOR2::operator+=;
	using D3DXVECTOR2::operator-=;
	using D3DXVECTOR2::operator*=;
	using D3DXVECTOR2::operator/=;
	using D3DXVECTOR2::operator+;
	using D3DXVECTOR2::operator-;
	using D3DXVECTOR2::operator*;
	using D3DXVECTOR2::operator/;
	using D3DXVECTOR2::operator==;
	using D3DXVECTOR2::operator!=;

	// デフォルトコンストラクタ
	VECTOR2() : D3DXVECTOR2(0.0f, 0.0f) {}

	// 引数付きコンストラクタ
	VECTOR2(const float XY)				: D3DXVECTOR2(XY, XY)			{}
	VECTOR2(const D3DXVECTOR2& VEC2)	: D3DXVECTOR2(VEC2.x, VEC2.y)	{}
	VECTOR2(const D3DXVECTOR3& VEC3)	: D3DXVECTOR2(VEC3.x, VEC3.y)	{}

	// デストラクタ
	~VECTOR2() {}

	//********************************************************
	//	演算子オーバーロード
	//********************************************************
	// 加算
	inline VECTOR2 operator+(const float fVal) const { return VECTOR2(x + fVal, y + fVal); }

	// 減算
	inline VECTOR2 operator-(const float fVal) const { return VECTOR2(x - fVal, y - fVal); }

	// 加算代入
	inline VECTOR2& operator+=(const float fVal)
	{
		x += fVal;
		y += fVal;
		return *this;
	}

	// 減算代入
	inline VECTOR2& operator-=(const float fVal)
	{
		x -= fVal;
		y -= fVal;
		return *this;
	}

	// 乗算代入
	inline VECTOR2& operator*=(const VECTOR2& rVec)
	{
		x *= rVec.x;
		y *= rVec.y;
		return *this;
	}

	// 除算代入
	inline VECTOR2& operator/=(const VECTOR2& rVec)
	{
		x /= rVec.x;
		y /= rVec.y;
		return *this;
	}

	//********************************************************
	//	メンバ関数
	//********************************************************
	/* @brief ベクトルの正規化 */
	inline void Normal() { D3DXVec2Normalize(this, this); }

	/* @brief ベクトルの反転 */
	inline void Invert() { *this = VECTOR2(-x, -y); }

	/* @brief ベクトルの絶対値 */
	inline void Absolute() { *this = VECTOR2(fabsf(x), fabsf(y)); }

	/*
		@brief	許容される誤差の範囲内にあるか
		@param	const float [in] 許容範囲
		@return	判定結果
	*/
	inline bool IsNearlyZero(const float fRange) const
	{
		if (fabsf(x) <= fRange && fabsf(y) <= fRange) { return true; }
		return false;
	}

	/*
		@brief	要素がすべてゼロか
		@return	判定結果
	*/
	inline bool IsZero() const
	{
		if (x == 0.0f && y == 0.0f) { return true; }
		return false;
	}
};

// 3次元ベクトル
struct VECTOR3 : public D3DXVECTOR3
{
	// コンストラクタ継承
	using D3DXVECTOR3::D3DXVECTOR3;

	// 演算子オーバーライド再公開
	using D3DXVECTOR3::operator+=;
	using D3DXVECTOR3::operator-=;
	using D3DXVECTOR3::operator*=;
	using D3DXVECTOR3::operator/=;
	using D3DXVECTOR3::operator+;
	using D3DXVECTOR3::operator-;
	using D3DXVECTOR3::operator*;
	using D3DXVECTOR3::operator/;
	using D3DXVECTOR3::operator==;
	using D3DXVECTOR3::operator!=;

	// デフォルトコンストラクタ
	VECTOR3() : D3DXVECTOR3(0.0f, 0.0f, 0.0f) {}

	// 引数付きコンストラクタ
	VECTOR3(const float XYZ)			: D3DXVECTOR3(XYZ, XYZ, XYZ)			{}
	VECTOR3(const D3DXVECTOR3& VEC3)	: D3DXVECTOR3(VEC3.x, VEC3.y, VEC3.z)	{}
	VECTOR3(const D3DXVECTOR2& VEC2)	: D3DXVECTOR3(VEC2.x, VEC2.y, 0.0f)		{}

	// デストラクタ
	~VECTOR3() {}

	//********************************************************
	//	演算子オーバーロード
	//********************************************************
	// 加算
	inline VECTOR3 operator+(const float fVal) const { return VECTOR3(x + fVal, y + fVal, z + fVal); }

	// 減算
	inline VECTOR3 operator-(const float fVal) const { return VECTOR3(x - fVal, y - fVal, z - fVal); }

	// 加算代入
	inline VECTOR3& operator+=(const float fVal)
	{
		x += fVal;
		y += fVal;
		z += fVal;
		return *this;
	}

	// 減算代入
	inline VECTOR3& operator-=(const float fVal)
	{
		x -= fVal;
		y -= fVal;
		z -= fVal;
		return *this;
	}

	// 乗算代入
	inline VECTOR3& operator*=(const VECTOR3& rVec)
	{
		x *= rVec.x;
		y *= rVec.y;
		z *= rVec.z;
		return *this;
	}

	// 除算代入
	inline VECTOR3& operator/=(const VECTOR3& rVec)
	{
		x /= rVec.x;
		y /= rVec.y;
		z /= rVec.z;
		return *this;
	}

	//********************************************************
	//	メンバ関数
	//********************************************************
	/* @brief ベクトルの正規化 */
	inline void Normal() { D3DXVec3Normalize(this, this); }

	/* @brief ベクトルの反転 */
	inline void Invert() { *this = VECTOR3(-x, -y, -z); }

	/* @brief ベクトルの絶対値 */
	inline void Absolute() { *this = VECTOR3(fabsf(x), fabsf(y), fabsf(z)); }

	/*
		@brief	許容される誤差の範囲内にあるか
		@param	const float [in] 許容範囲
		@return	判定結果
	*/
	inline bool IsNearlyZero(const float fRange) const
	{
		if (fabsf(x) <= fRange && fabsf(y) <= fRange && fabsf(z) <= fRange) { return true; }
		return false;
	}

	/*
		@brief	要素がすべてゼロか
		@return	判定結果
	*/
	inline bool IsZero() const
	{
		if (x == 0.0f && y == 0.0f && z == 0.0f) { return true; }
		return false;
	}
};

// 2整数座標
struct POSGRID2
{
	// デフォルトコンストラクタ
	POSGRID2() : x(0), y(0) {}

	// 引数付きコンストラクタ
	POSGRID2(const int XY)				: x(XY), y(XY)	{}
	POSGRID2(const int X, const int Y)	: x(X), y(Y)	{}

	// デストラクタ
	~POSGRID2() {}

	// メンバ変数
	int x;	// 方眼座標 (x)
	int y;	// 方眼座標 (y)

	//********************************************************
	//	演算子オーバーロード
	//********************************************************
	// 加算
	inline POSGRID2 operator+(const POSGRID2& rGrid) const	{ return POSGRID2(x + rGrid.x, y + rGrid.y); }
	inline POSGRID2 operator+(const int nVal) const			{ return POSGRID2(x + nVal, y + nVal); }
	inline POSGRID2 operator+() const						{ return *this; }

	// 減算
	inline POSGRID2 operator-(const POSGRID2& rGrid) const	{ return POSGRID2(x - rGrid.x, y - rGrid.y); }
	inline POSGRID2 operator-(const int nVal) const			{ return POSGRID2(x - nVal, y - nVal); }
	inline POSGRID2 operator-() const						{ return POSGRID2(-x, -y); }

	// 乗算
	inline POSGRID2 operator*(const int nVal) const	{ return POSGRID2(x * nVal, y * nVal); }

	// 除算
	inline POSGRID2 operator/(const int nVal) const	{ return POSGRID2(x / nVal, y / nVal); }

	// 加算代入
	inline POSGRID2& operator+=(const int nVal)
	{
		x += nVal;
		y += nVal;
		return *this;
	}
	inline POSGRID2& operator+=(const POSGRID2& rGrid)
	{
		x += rGrid.x;
		y += rGrid.y;
		return *this;
	}

	// 減算代入
	inline POSGRID2& operator-=(const int nVal)
	{
		x -= nVal;
		y -= nVal;
		return *this;
	}
	inline POSGRID2& operator-=(const POSGRID2& rGrid)
	{
		x -= rGrid.x;
		y -= rGrid.y;
		return *this;
	}

	// 乗算代入
	inline POSGRID2& operator*=(const int nVal)
	{
		x *= nVal;
		y *= nVal;
		return *this;
	}
	inline POSGRID2& operator*=(const POSGRID2& rGrid)
	{
		x *= rGrid.x;
		y *= rGrid.y;
		return *this;
	}

	// 除算代入
	inline POSGRID2& operator/=(const int nVal)
	{
		x /= nVal;
		y /= nVal;
		return *this;
	}
	inline POSGRID2& operator/=(const POSGRID2& rGrid)
	{
		x /= rGrid.x;
		y /= rGrid.y;
		return *this;
	}

	/*
		@brief	座標が一致するかの検証
		@param	const POSGRID2& [in] 判定する方眼座標
		@return	判定結果
	*/
	inline bool operator==(const POSGRID2& rGrid) const
	{
		if (rGrid.x == x && rGrid.y == y) { return true; }
		return false;
	}

	/*
		@brief	座標が一致しないかの検証
		@param	const POSGRID2& [in] 判定する方眼座標
		@return	判定結果
	*/
	inline bool operator!=(const POSGRID2& rGrid) const
	{
		if (rGrid.x != x || rGrid.y != y) { return true; }
		return false;
	}

	//********************************************************
	//	メンバ関数
	//********************************************************
	/* @brief 方眼座標の反転 */
	inline void Invert() { *this = POSGRID2(-x, -y); }

	/* @brief 方眼座標の絶対値 */
	inline void Absolute() { *this = POSGRID2(std::abs(x), std::abs(y)); }

	/*
		@brief	要素がすべてゼロか
		@return	判定結果
	*/
	inline bool IsZero() const
	{
		if (x == 0 && y == 0) { return true; }
		return false;
	}
};

// 3整数座標
struct POSGRID3
{
	// デフォルトコンストラクタ
	POSGRID3() : x(0), y(0), z(0) {}

	// 引数付きコンストラクタ
	POSGRID3(const int XYZ)							: x(XYZ), y(XYZ), z(XYZ)	{}
	POSGRID3(const int X, const int Y, const int Z)	: x(X), y(Y), z(Z)			{}

	// デストラクタ
	~POSGRID3() {}

	// メンバ変数
	int x;	// 方眼座標 (x)
	int y;	// 方眼座標 (y)
	int z;	// 方眼座標 (z)

	//********************************************************
	//	演算子オーバーロード
	//********************************************************
	// 加算
	inline POSGRID3 operator+(const POSGRID3& rGrid) const	{ return POSGRID3(x + rGrid.x, y + rGrid.y, z + rGrid.z); }
	inline POSGRID3 operator+(const int nVal) const			{ return POSGRID3(x + nVal, y + nVal, z + nVal); }
	inline POSGRID3 operator+() const						{ return *this; }

	// 減算
	inline POSGRID3 operator-(const POSGRID3& rGrid) const	{ return POSGRID3(x - rGrid.x, y - rGrid.y, z - rGrid.z); }
	inline POSGRID3 operator-(const int nVal) const			{ return POSGRID3(x - nVal, y - nVal, z - nVal); }
	inline POSGRID3 operator-() const						{ return POSGRID3(-x, -y, -z); }

	// 乗算
	inline POSGRID3 operator*(const int nVal) const	{ return POSGRID3(x * nVal, y * nVal, z * nVal); }

	// 除算
	inline POSGRID3 operator/(const int nVal) const	{ return POSGRID3(x / nVal, y / nVal, z / nVal); }

	// 加算代入
	inline POSGRID3& operator+=(const int nVal)
	{
		x += nVal;
		y += nVal;
		z += nVal;
		return *this;
	}
	inline POSGRID3& operator+=(const POSGRID3& rGrid)
	{
		x += rGrid.x;
		y += rGrid.y;
		z += rGrid.z;
		return *this;
	}

	// 減算代入
	inline POSGRID3& operator-=(const int nVal)
	{
		x -= nVal;
		y -= nVal;
		z -= nVal;
		return *this;
	}
	inline POSGRID3& operator-=(const POSGRID3& rGrid)
	{
		x -= rGrid.x;
		y -= rGrid.y;
		z -= rGrid.z;
		return *this;
	}

	// 乗算代入
	inline POSGRID3& operator*=(const int nVal)
	{
		x *= nVal;
		y *= nVal;
		z *= nVal;
		return *this;
	}
	inline POSGRID3& operator*=(const POSGRID3& rGrid)
	{
		x *= rGrid.x;
		y *= rGrid.y;
		z *= rGrid.z;
		return *this;
	}

	// 除算代入
	inline POSGRID3& operator/=(const int nVal)
	{
		x /= nVal;
		y /= nVal;
		z /= nVal;
		return *this;
	}
	inline POSGRID3& operator/=(const POSGRID3& rGrid)
	{
		x /= rGrid.x;
		y /= rGrid.y;
		z /= rGrid.z;
		return *this;
	}

	/*
		@brief	座標が一致するかの検証
		@param	const POSGRID3& [in] 判定する方眼座標
		@return	判定結果
	*/
	inline bool operator==(const POSGRID3& rGrid) const
	{
		if (rGrid.x == x && rGrid.y == y && rGrid.z == z) { return true; }
		return false;
	}

	/*
		@brief	座標が一致しないかの検証
		@param	const POSGRID3& [in] 判定する方眼座標
		@return	判定結果
	*/
	inline bool operator!=(const POSGRID3& rGrid) const
	{
		if (rGrid.x != x || rGrid.y != y || rGrid.z != z) { return true; }
		return false;
	}

	//********************************************************
	//	メンバ関数
	//********************************************************
	/* @brief 方眼座標の反転 */
	inline void Invert() { *this = POSGRID3(-x, -y, -z); }

	/* @brief 方眼座標の絶対値 */
	inline void Absolute() { *this = POSGRID3(std::abs(x), std::abs(y), std::abs(z)); }

	/*
		@brief	要素がすべてゼロか
		@return	判定結果
	*/
	inline bool IsZero() const
	{
		if (x == 0 && y == 0 && z == 0) { return true; }
		return false;
	}
};

// マトリックス
struct MATRIX : public D3DXMATRIX
{
	// コンストラクタ継承
	using D3DXMATRIX::D3DXMATRIX;

	// 演算子オーバーライド再公開
	using D3DXMATRIX::operator+=;
	using D3DXMATRIX::operator-=;
	using D3DXMATRIX::operator*=;
	using D3DXMATRIX::operator/=;
	using D3DXMATRIX::operator+;
	using D3DXMATRIX::operator-;
	using D3DXMATRIX::operator*;
	using D3DXMATRIX::operator/;
	using D3DXMATRIX::operator==;
	using D3DXMATRIX::operator!=;

	// デフォルトコンストラクタ
	MATRIX() : D3DXMATRIX(1.0f, 0.0f, 0.0f, 0.0f,
						  0.0f, 1.0f, 0.0f, 0.0f,
						  0.0f, 0.0f, 1.0f, 0.0f,
						  0.0f, 0.0f, 0.0f, 1.0f) {}

	// 引数付きコンストラクタ
	MATRIX(const D3DXMATRIX& MTX) : D3DXMATRIX(MTX) {}

	// デストラクタ
	~MATRIX() {}

	//********************************************************
	//	メンバ関数
	//********************************************************
	/* @brief 単位マトリックス化 */
	inline void Identity() { D3DXMatrixIdentity(this); }

	/* @brief 逆マトリックス化 */
	inline void Inverse() { D3DXMatrixInverse(this, nullptr, this); }

	/*
		@brief	マトリックスを掛け合わせる
		@param	const MATRIX& [in] 掛け合わせるマトリックス
		@param	const MATRIX& [in] 掛け合わせるマトリックス
	*/
	inline void Multiply(const MATRIX& rMtx0, const MATRIX& rMtx1) { D3DXMatrixMultiply(this, &rMtx0, &rMtx1); }

	/*
		@brief	位置マトリックスの計算
		@param	const VECTOR3& [in] 参照する位置
	*/
	inline void Translation(const VECTOR3& rPos) { D3DXMatrixTranslation(this, rPos.x, rPos.y, rPos.z); }

	/*
		@brief	位置マトリックスの計算
		@param	const float [in] 参照するX座標
		@param	const float [in] 参照するY座標
		@param	const float [in] 参照するZ座標
	*/
	inline void Translation(const float fX, const float fY, const float fZ) { D3DXMatrixTranslation(this, fX, fY, fZ); }

	/*
		@brief	向きマトリックスの計算
		@param	const VECTOR3& [in] 参照する向き
	*/
	inline void Rotation(const VECTOR3& rRot) { D3DXMatrixRotationYawPitchRoll(this, rRot.y, rRot.x, rRot.z); }

	/*
		@brief	向きマトリックスの計算
		@param	const float [in] 参照するYaw向き
		@param	const float [in] 参照するPitch向き
		@param	const float [in] 参照するRoll向き
	*/
	inline void Rotation(const float fYaw, const float fPitch, const float fRoll) { D3DXMatrixRotationYawPitchRoll(this, fYaw, fPitch, fRoll); }

	/*
		@brief	拡大率マトリックスの計算
		@param	const VECTOR3& [in] 参照する拡大率
	*/
	inline void Scaling(const VECTOR3& rScale) { D3DXMatrixScaling(this, rScale.x, rScale.y, rScale.z); }

	/*
		@brief	拡大率マトリックスの計算
		@param	const float [in] 参照するX拡大率
		@param	const float [in] 参照するY拡大率
		@param	const float [in] 参照するZ拡大率
	*/
	inline void Scaling(const float fX, const float fY, const float fZ) { D3DXMatrixScaling(this, fX, fY, fZ); }

	/*
		@brief	マトリックス位置の取得
		@return	マトリックスの位置
	*/
	inline VECTOR3 GetPosition() const { return VECTOR3(this->_41, this->_42, this->_43); }

	/*
		@brief	マトリックス向きの取得
		@return	マトリックスの向き
	*/
	inline VECTOR3 GetRotation() const
	{
		float fYaw, fPitch, fRoll;	// 計算結果の保存用
		float fCosPitch;			// 向き計算用

		// マトリックスからPitchを求める
		fPitch = asinf(-(*this)._32);

		// マトリックスからYaw・Rollを求める
		fCosPitch = cosf(fPitch);
		if (fabs(fCosPitch) > 0.0001f)
		{ // Pitchの角度が計算に問題ない場合

			fYaw  = atan2f(this->_31 / fCosPitch, this->_33 / fCosPitch);
			fRoll = atan2f(this->_12 / fCosPitch, this->_22 / fCosPitch);
		}
		else
		{ // Pitchが90度または-90度の場合

			fYaw  = 0.0f;	// 正確な値が取れないので0.0fとする
			fRoll = atan2f(this->_21, this->_11);
		}

		// マトリックスの向きを返す
		return VECTOR3(fPitch, fYaw, fRoll);
	}

	/*
		@brief	マトリックス拡大率の取得
		@return	マトリックスの拡大率
	*/
	inline VECTOR3 GetScale() const
	{
		VECTOR3 scale;
		scale.x = sqrtf(this->_11 * this->_11 + this->_12 * this->_12 + this->_13 * this->_13);
		scale.y = sqrtf(this->_21 * this->_21 + this->_22 * this->_22 + this->_23 * this->_23);
		scale.z = sqrtf(this->_31 * this->_31 + this->_32 * this->_32 + this->_33 * this->_33);
		return scale;
	}
};

// 色
struct COLOR : public D3DXCOLOR
{
	// コンストラクタ継承
	using D3DXCOLOR::D3DXCOLOR;

	// 演算子オーバーライド再公開
	using D3DXCOLOR::operator+=;
	using D3DXCOLOR::operator-=;
	using D3DXCOLOR::operator*=;
	using D3DXCOLOR::operator/=;
	using D3DXCOLOR::operator+;
	using D3DXCOLOR::operator-;
	using D3DXCOLOR::operator*;
	using D3DXCOLOR::operator/;
	using D3DXCOLOR::operator==;
	using D3DXCOLOR::operator!=;

	// デフォルトコンストラクタ
	COLOR() : D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f) {}

	// 引数付きコンストラクタ
	COLOR(const float ALL)						: D3DXCOLOR(ALL, ALL, ALL, ALL)		{}
	COLOR(const float COL, const float ALPHA)	: D3DXCOLOR(COL, COL, COL, ALPHA)	{}
	COLOR(const D3DXCOLOR& COL)					: D3DXCOLOR(COL)					{}

	// デストラクタ
	~COLOR() {}

	//********************************************************
	//	メンバ関数
	//********************************************************
	inline void Black()						{ *this = COLOR(0.0f, 0.0f, 0.0f, 1.0f); }		// 黒色/不透明
	inline void Black(const float fAlpha)	{ *this = COLOR(0.0f, 0.0f, 0.0f, fAlpha); }	// 黒色/透明度
	inline void White()						{ *this = COLOR(1.0f, 1.0f, 1.0f, 1.0f); }		// 白色/不透明
	inline void White(const float fAlpha)	{ *this = COLOR(1.0f, 1.0f, 1.0f, fAlpha); }	// 白色/透明度
	inline void Red()						{ *this = COLOR(1.0f, 0.0f, 0.0f, 1.0f); }		// 赤色/不透明
	inline void Red(const float fAlpha)		{ *this = COLOR(1.0f, 0.0f, 0.0f, fAlpha); }	// 赤色/透明度
	inline void Green()						{ *this = COLOR(0.0f, 1.0f, 0.0f, 1.0f); }		// 緑色/不透明
	inline void Green(const float fAlpha)	{ *this = COLOR(0.0f, 1.0f, 0.0f, fAlpha); }	// 緑色/透明度
	inline void Blue()						{ *this = COLOR(0.0f, 0.0f, 1.0f, 1.0f); }		// 青色/不透明
	inline void Blue(const float fAlpha)	{ *this = COLOR(0.0f, 0.0f, 1.0f, fAlpha); }	// 青色/透明度
	inline void Yellow()					{ *this = COLOR(1.0f, 1.0f, 0.0f, 1.0f); }		// 黄色/不透明
	inline void Yellow(const float fAlpha)	{ *this = COLOR(1.0f, 1.0f, 0.0f, fAlpha); }	// 黄色/透明度
	inline void Cyan()						{ *this = COLOR(0.0f, 1.0f, 1.0f, 1.0f); }		// 水色/不透明
	inline void Cyan(const float fAlpha)	{ *this = COLOR(0.0f, 1.0f, 1.0f, fAlpha); }	// 水色/透明度
	inline void Purple()					{ *this = COLOR(1.0f, 0.0f, 1.0f, 1.0f); }		// 紫色/不透明
	inline void Purple(const float fAlpha)	{ *this = COLOR(1.0f, 0.0f, 1.0f, fAlpha); }	// 紫色/透明度

	/*
		@brief	カラーコードから色/透明度の設定
		@param	const std::string&	[in] カラーコード文字列
		@param	const float			[in] 透明度
	*/
	inline void Code(const std::string& rCode, const float fAlpha)
	{
		assert((int)rCode.size() == 6);
		int R = std::stoi(rCode.substr(0, 2), nullptr, 16);
		int G = std::stoi(rCode.substr(2, 2), nullptr, 16);
		int B = std::stoi(rCode.substr(4, 2), nullptr, 16);
		*this = D3DCOLOR_RGBA(R, G, B, (int)(fAlpha * 255.0f));
	}

	/*
		@brief	カラーコードから色の設定
		@param	const std::string&	[in] カラーコード文字列
	*/
	inline void Code(const std::string& rCode)
	{
		this->Code(rCode, 1.0f);
	}

	/*
		@brief	透明度がゼロか
		@return	判定結果
	*/
	inline bool IsAlpha() const
	{
		if (a <= 0.0f) { return true; }
		return false;
	}
};

//************************************************************
//	エイリアス定義
//************************************************************
using AText = std::vector<std::string>;	// テキスト保存型
using ATextBox = std::vector<AText>;	// 複数テキスト保存型

//************************************************************
//	名前空間宣言
//************************************************************
// 便利関数空間
namespace useful
{
	// 通常関数
	void DivideDigitNum		// 数値の桁数ごとの分解
	( // 引数
		int* pNumDivide,	// 分解結果の格納配列
		const int nNum,		// 分解する数値
		const int nMaxDigit	// 分解する数字の桁数
	);
	void NormalizeNormal	// 法線の正規化
	( // 引数
		const VECTOR3& rPosLeft,	// 左位置
		const VECTOR3& rPosCenter,	// 中心位置
		const VECTOR3& rPosRight,	// 右位置
		VECTOR3& rNor				// 法線
	);
	void ReplaceConsecChar	// 文字列内の連続文字の置換
	( // 引数
		std::string* pDestStr,		// 置き換えを行う文字列
		const char cRepChar,		// 検出する文字
		const std::string& rRepStr,	// 置き換える文字列
		const int nRepStrLength		// 置き換える文字列の長さ (通常はstringのsize)
	);
	VECTOR3 CalcParabola3D	// 放物線計算処理
	( // 引数
		const VECTOR3& rPosStart,	// 射出位置
		const VECTOR3& rPosEnd,		// 着弾位置
		const float fMaxHeight,		// 最高到達高さ
		const float fEndTime,		// 終了時間
		const float fCurTime		// 現在時間
	);

	float RandomRot();					// ランダム向き取得
	void NormalizeRot(float& rRot);		// 向きの正規化
	void NormalizeRot(VECTOR3& rRot);	// 三軸向きの正規化
	void PathToBaseName(std::string* pPath);		// パスのベースネーム変換
	void StandardizePathPart(std::string* pPath);	// パス区切りの標準化
	std::string SandString(const std::string& rTop, const std::string& rMain, const std::string& rCur);		// 文字列の左右追加 (マルチバイト文字列)
	std::wstring SandString(const std::wstring& rTop, const std::wstring& rMain, const std::wstring& rCur);	// 文字列の左右追加 (ワイド文字列)
	std::wstring MultiByteToWide(const std::string& rSrcStr);	// マルチバイト文字列のワイド文字列変換
	std::string WideToMultiByte(const std::wstring& rSrcStr);	// ワイド文字列のマルチバイト文字列変換
	void VecToRot(const VECTOR3& rVec, float* pPhi, float* pTheta);		// ベクトルの向き変換
	void RotToVec(const float fPhi, const float fTheta, VECTOR3* pVec);	// 向きのベクトル変換
	float GetTexWidthFromAspect(const float fHeight, const char* pTexturePath);	// 縦幅からアスペクト比を考慮した横幅取得
	float GetTexWidthFromAspect(const float fHeight, const int nTexIdx);		// 縦幅からアスペクト比を考慮した横幅取得
	float GetTexHeightFromAspect(const float fWidth, const char* pTexturePath);	// 横幅からアスペクト比を考慮した縦幅取得
	float GetTexHeightFromAspect(const float fWidth, const int nTexIdx);		// 横幅からアスペクト比を考慮した縦幅取得

	// テンプレート関数
	template<class T> T* ZeroClear(T* pClear);	// ゼロクリア
	template<class T> bool LimitNum		// 値の範囲内制限
	( // 引数
		T& rNum,		// 制限数値
		const T min,	// 最小範囲
		const T max		// 最大範囲
	);
	template<class T> bool LimitMinNum	// 値の最小値制限
	( // 引数
		T& rNum,		// 制限数値
		const T min		// 最小範囲
	);
	template<class T> bool LimitMaxNum	// 値の最大値制限
	( // 引数
		T& rNum,		// 制限数値
		const T max		// 最大範囲
	);
	template<class T> float ValueToRate	// 値の割合変換
	( // 引数
		const T num,	// 割合化する数値
		const T min,	// 最小範囲
		const T max		// 最大範囲
	);
	template<class T> T RateToValue		// 割合の値変換
	( // 引数
		const float fRate,	// 値化する数値
		const T min,		// 最小範囲
		const T max			// 最大範囲
	);
	template<class T> void SortNum		// 値のソート
	( // 引数
		T* pSortNum,		// ソート配列
		const int nMaxKeep	// 配列サイズ
	);
	template<class T> void Shuffle		// シャッフル
	(
		T* pShuffleData,		// シャッフル配列
		const int nMaxShuffle	// 配列サイズ
	);
}

// イージング関数空間
namespace easing
{
	// 通常関数
	inline float Liner(const float x)		{ return x; }
	inline float InSine(const float x)		{ return 1.0f - cosf((x * D3DX_PI) * 0.5f); }
	inline float OutSine(const float x)		{ return sinf((x * D3DX_PI) * 0.5f); }
	inline float InOutSine(const float x)	{ return -(cosf(x * D3DX_PI) - 1.0f) * 0.5f; }
	inline float InQuad(const float x)		{ return x * x; }
	inline float OutQuad(const float x)		{ return 1.0f - (1.0f - x) * (1.0f - x); }
	inline float InOutQuad(const float x)	{ return (x < 0.5f) ? (2.0f * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 2.0f) * 0.5f); }
	inline float InCubic(const float x)		{ return x * x * x; }
	inline float OutCubic(const float x)	{ return 1.0f - powf(1.0f - x, 3.0f); }
	inline float InOutCubic(const float x)	{ return (x < 0.5f) ? (4.0f * x * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 3.0f) * 0.5f); }
	inline float InQuart(const float x)		{ return x * x * x * x; }
	inline float OutQuart(const float x)	{ return 1.0f - powf(1.0f - x, 4.0f); }
	inline float InOutQuart(const float x)	{ return (x < 0.5f) ? (8.0f * x * x * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 4.0f) * 0.5f); }
	inline float InQuint(const float x)		{ return x * x * x * x * x; }
	inline float OutQuint(const float x)	{ return 1.0f - powf(1.0f - x, 5.0f); }
	inline float InOutQuint(const float x)	{ return (x < 0.5f) ? (16.0f * x * x * x * x * x) : (1.0f - powf(-2.0f * x + 2.0f, 5.0f) * 0.5f); }
	inline float InElastic(const float x)	{ return (x <= 0.0f) ? 0.0f : (x >= 1.0f) ? 1.0f : -powf(2.0f, 10.0f * (x - 1.0f)) * sinf((x - 1.1f) * 5.0f * D3DX_PI); }
	inline float OutElastic(const float x)	{ return (x <= 0.0f) ? 0.0f : (x >= 1.0f) ? 1.0f : powf(2.0f, -10.0f * x) * sinf((x - 0.1f) * 5.0f * D3DX_PI) + 1.0f; }
	inline float InOutElastic(const float x)
	{
		const float c5 = (2.0f * D3DX_PI) / 4.5f;
		if		(x <= 0.0f)	{ return 0.0f; }
		else if	(x >= 1.0f)	{ return 1.0f; }
		if		(x < 0.5f)	{ return -(powf(2.0f, 20.0f * x - 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f; }
		else				{ return (powf(2.0f, -20.0f * x + 10.0f) * sinf((20.0f * x - 11.125f) * c5)) / 2.0f + 1.0f; }
	}
	inline float InBack(const float x, const float c1 = 1.70158f)	{ return (c1 + 1.0f) * x * x * x - c1 * x * x; }
	inline float OutBack(const float x, const float c1 = 1.70158f)	{ return 1.0f + (c1 + 1.0f) * powf(x - 1.0f, 3.0f) + c1 * powf(x - 1.0f, 2.0f); }
	inline float InOutBack(const float x, const float c1 = 1.70158f)
	{
		const float c2 = c1 * 1.525f;
		if (x < 0.5f)	{ return (powf(2.0f * x, 2.0f) * ((c2 + 1.0f) * 2.0f * x - c2)) / 2.0f; }
		else			{ return (powf(2.0f * x - 2.0f, 2.0f) * ((c2 + 1.0f) * (x * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f; }
	}

	// テンプレート関数
	template<class T> inline float Liner(const T num, const T min, const T max)			{ return Liner(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InSine(const T num, const T min, const T max)		{ return InSine(useful::ValueToRate(num, min, max)); }
	template<class T> inline float OutSine(const T num, const T min, const T max)		{ return OutSine(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InOutSine(const T num, const T min, const T max)		{ return InOutSine(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InQuad(const T num, const T min, const T max)		{ return InQuad(useful::ValueToRate(num, min, max)); }
	template<class T> inline float OutQuad(const T num, const T min, const T max)		{ return OutQuad(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InOutQuad(const T num, const T min, const T max)		{ return InOutQuad(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InCubic(const T num, const T min, const T max)		{ return InCubic(useful::ValueToRate(num, min, max)); }
	template<class T> inline float OutCubic(const T num, const T min, const T max)		{ return OutCubic(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InOutCubic(const T num, const T min, const T max)	{ return InOutCubic(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InQuart(const T num, const T min, const T max)		{ return InQuart(useful::ValueToRate(num, min, max)); }
	template<class T> inline float OutQuart(const T num, const T min, const T max)		{ return OutQuart(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InOutQuart(const T num, const T min, const T max)	{ return InOutQuart(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InQuint(const T num, const T min, const T max)		{ return InQuint(useful::ValueToRate(num, min, max)); }
	template<class T> inline float OutQuint(const T num, const T min, const T max)		{ return OutQuint(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InOutQuint(const T num, const T min, const T max)	{ return InOutQuint(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InElastic(const T num, const T min, const T max)		{ return InElastic(useful::ValueToRate(num, min, max)); }
	template<class T> inline float OutElastic(const T num, const T min, const T max)	{ return OutElastic(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InOutElastic(const T num, const T min, const T max)	{ return InOutElastic(useful::ValueToRate(num, min, max)); }
	template<class T> inline float InBack(const T num, const T min, const T max, const float c1 = 1.70158f)		{ return InBack(useful::ValueToRate(num, min, max), c1); }
	template<class T> inline float OutBack(const T num, const T min, const T max, const float c1 = 1.70158f)	{ return OutBack(useful::ValueToRate(num, min, max), c1); }
	template<class T> inline float InOutBack(const T num, const T min, const T max, const float c1 = 1.70158f)	{ return InOutBack(useful::ValueToRate(num, min, max), c1); }
	template<class T> inline T Liner(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = Liner(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T InSine(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = InSine(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T OutSine(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = OutSine(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T InOutSine(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InOutSine(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値の計算
	}
	template<class T> inline T InQuad(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = InQuad(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T OutQuad(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = OutQuad(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T InOutQuad(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InOutQuad(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値の計算
	}
	template<class T> inline T InCubic(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = InCubic(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T OutCubic(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = OutCubic(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T InOutCubic(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InOutCubic(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値の計算
	}
	template<class T> inline T InQuart(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = InQuart(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T OutQuart(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = OutQuart(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T InOutQuart(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InOutQuart(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値の計算
	}
	template<class T> inline T InQuint(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = InQuint(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T OutQuint(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);		// 現在時間の補正
		float fRate = OutQuint(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;					// 割合から現在値の計算
	}
	template<class T> inline T InOutQuint(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InOutQuint(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値の計算
	}
	template<class T> inline T InElastic(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InElastic(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値計算
	}
	template<class T> inline T OutElastic(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = OutElastic(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値計算
	}
	template<class T> inline T InOutElastic(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InOutElastic(fCurTime, fStartTime, fEndTime);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値計算
	}
	template<class T> inline T InBack(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime, const float c1 = 1.70158f)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = InBack(fCurTime, fStartTime, fEndTime, c1);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値計算
	}
	template<class T> inline T OutBack(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime, const float c1 = 1.70158f)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);			// 現在時間の補正
		float fRate = OutBack(fCurTime, fStartTime, fEndTime, c1);	// 線形補間の割合計算
		return start + (end - start) * fRate;						// 割合から現在値計算
	}
	template<class T> inline T InOutBack(const T start, const T end, const float fStartTime, const float fEndTime, float fCurTime, const float c1 = 1.70158f)
	{
		useful::LimitNum(fCurTime, fStartTime, fEndTime);				// 現在時間の補正
		float fRate = InOutBack(fCurTime, fStartTime, fEndTime, c1);	// 線形補間の割合計算
		return start + (end - start) * fRate;							// 割合から現在値計算
	}
}

// カラー関数空間
namespace color
{
	inline COLOR Black()					{ return COLOR(0.0f, 0.0f, 0.0f, 1.0f); }	// 黒色/不透明
	inline COLOR Black(const float fAlpha)	{ return COLOR(0.0f, 0.0f, 0.0f, fAlpha); }	// 黒色/透明度
	inline COLOR White()					{ return COLOR(1.0f, 1.0f, 1.0f, 1.0f); }	// 白色/不透明
	inline COLOR White(const float fAlpha)	{ return COLOR(1.0f, 1.0f, 1.0f, fAlpha); }	// 白色/透明度
	inline COLOR Red()						{ return COLOR(1.0f, 0.0f, 0.0f, 1.0f); }	// 赤色/不透明
	inline COLOR Red(const float fAlpha)	{ return COLOR(1.0f, 0.0f, 0.0f, fAlpha); }	// 赤色/透明度
	inline COLOR Green()					{ return COLOR(0.0f, 1.0f, 0.0f, 1.0f); }	// 緑色/不透明
	inline COLOR Green(const float fAlpha)	{ return COLOR(0.0f, 1.0f, 0.0f, fAlpha); }	// 緑色/透明度
	inline COLOR Blue()						{ return COLOR(0.0f, 0.0f, 1.0f, 1.0f); }	// 青色/不透明
	inline COLOR Blue(const float fAlpha)	{ return COLOR(0.0f, 0.0f, 1.0f, fAlpha); }	// 青色/透明度
	inline COLOR Yellow()					{ return COLOR(1.0f, 1.0f, 0.0f, 1.0f); }	// 黄色/不透明
	inline COLOR Yellow(const float fAlpha)	{ return COLOR(1.0f, 1.0f, 0.0f, fAlpha); }	// 黄色/透明度
	inline COLOR Cyan()						{ return COLOR(0.0f, 1.0f, 1.0f, 1.0f); }	// 水色/不透明
	inline COLOR Cyan(const float fAlpha)	{ return COLOR(0.0f, 1.0f, 1.0f, fAlpha); }	// 水色/透明度
	inline COLOR Purple()					{ return COLOR(1.0f, 0.0f, 1.0f, 1.0f); }	// 紫色/不透明
	inline COLOR Purple(const float fAlpha)	{ return COLOR(1.0f, 0.0f, 1.0f, fAlpha); }	// 紫色/透明度

	// カラーコード/不透明
	inline COLOR Code(const std::string& rCode, const float fAlpha)
	{
		assert((int)rCode.size() == 6);
		int R = std::stoi(rCode.substr(0, 2), nullptr, 16);
		int G = std::stoi(rCode.substr(2, 2), nullptr, 16);
		int B = std::stoi(rCode.substr(4, 2), nullptr, 16);
		COLOR col = D3DCOLOR_RGBA(R, G, B, (int)(fAlpha * 255.0f));
		return col;
	}

	// カラーコード/透明度
	inline COLOR Code(const std::string& rCode)
	{
		return Code(rCode, 1.0f);
	}
}

// マテリアル関数空間
namespace material
{
	// 黒色マテリアル取得
	inline D3DXMATERIAL Black()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Black();
		return mat;
	}

	// 白色マテリアル取得
	inline D3DXMATERIAL White()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::White();
		return mat;
	}

	// 赤色マテリアル取得
	inline D3DXMATERIAL Red()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Red();
		return mat;
	}

	// 緑色マテリアル取得
	inline D3DXMATERIAL Green()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Green();
		return mat;
	}

	// 青色マテリアル取得
	inline D3DXMATERIAL Blue()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Blue();
		return mat;
	}

	// 黄色マテリアル取得
	inline D3DXMATERIAL Yellow()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Yellow();
		return mat;
	}

	// 水色マテリアル取得
	inline D3DXMATERIAL Cyan()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Cyan();
		return mat;
	}

	// 紫色マテリアル取得
	inline D3DXMATERIAL Purple()
	{
		D3DXMATERIAL mat;
		ZeroMemory(&mat, sizeof(mat));
		mat.MatD3D.Diffuse = color::Purple();
		return mat;
	}
}

//************************************************************
//	テンプレート関数
//************************************************************
//	便利関数空間
//************************************************************
//============================================================
//	ゼロクリア
//============================================================
template<class T> T* useful::ZeroClear(T* pClear)
{
	// 引数情報をゼロクリア
	memset(pClear, 0, sizeof(*pClear));

	// クリア後のアドレスを返す
	return pClear;
}

//============================================================
//	値の範囲内制限処理
//============================================================
template<class T> bool useful::LimitNum
(
	T& rNum,		// 制限数値
	const T min,	// 最小範囲
	const T max		// 最大範囲
)
{
	if (rNum < min)
	{ // 制限数値が最小範囲を超えていた場合

		// 範囲内に補正
		rNum = min;
		return true;
	}
	else if (rNum > max)
	{ // 制限数値が最大範囲を超えていた場合

		// 範囲内に補正
		rNum = max;
		return true;
	}

	return false;
}

//============================================================
//	値の最小値制限処理
//============================================================
template<class T> bool useful::LimitMinNum
(
	T& rNum,	// 制限数値
	const T min	// 最小範囲
)
{
	if (rNum < min)
	{ // 制限数値が最小範囲を超えていた場合

		// 範囲内に補正
		rNum = min;
		return true;
	}

	return false;
}

//============================================================
//	値の最大値制限処理
//============================================================
template<class T> bool useful::LimitMaxNum
(
	T& rNum,	// 制限数値
	const T max	// 最大範囲
)
{
	if (rNum > max)
	{ // 制限数値が最大範囲を超えていた場合

		// 範囲内に補正
		rNum = max;
		return true;
	}

	return false;
}

//============================================================
//	値の割合変換
//============================================================
template<class T> float useful::ValueToRate
(
	const T num,	// 割合化する数値
	const T min,	// 最小範囲
	const T max		// 最大範囲
)
{
	// 割る数を求める
	float fDiv = static_cast<float>(max) - static_cast<float>(min);
	if (fDiv == 0.0f) { return 0.0f; }	// 0除算対策

	// 割合変換した値を返す
	return (static_cast<float>(num) - static_cast<float>(min)) / fDiv;
}

//============================================================
//	割合の値変換
//============================================================
template<class T> T useful::RateToValue
(
	const float fRate,	// 値化する数値
	const T min,		// 最小範囲
	const T max			// 最大範囲
)
{
	// 値変換した割合を返す
	return static_cast<T>((fRate * (max - min)) + min);
}

//============================================================
//	ソート処理
//============================================================
template<class T> void useful::SortNum
(
	T* pSortNum,		// ソート配列
	const int nMaxKeep	// 配列サイズ
)
{
	T	keepNum;		// ソート用
	int	nCurrentMaxIdx;	// 最大値のインデックス
	for (int nCntKeep = 0; nCntKeep < (nMaxKeep - 1); nCntKeep++)
	{ // 入れ替える値の総数 -1回分繰り返す

		// 現在の繰り返し数を代入 (要素1とする)
		nCurrentMaxIdx = nCntKeep;

		for (int nCntSort = nCntKeep + 1; nCntSort < nMaxKeep; nCntSort++)
		{ // 入れ替える値の総数 -nCntKeep分繰り返す

			if (pSortNum[nCurrentMaxIdx] < pSortNum[nCntSort])
			{ // 最大値に設定されている値より、現在の値のほうが大きい場合

				// 現在の要素番号を最大値に設定
				nCurrentMaxIdx = nCntSort;
			}
		}

		if (nCntKeep != nCurrentMaxIdx)
		{ // 最大値の要素番号に変動があった場合

			// 要素の入れ替え
			keepNum						= pSortNum[nCntKeep];
			pSortNum[nCntKeep]			= pSortNum[nCurrentMaxIdx];
			pSortNum[nCurrentMaxIdx]	= keepNum;
		}
	}
}

//============================================================
//	シャッフル処理
//============================================================
template<class T> void useful::Shuffle
(
	T* pShuffleData,		// シャッフル配列
	const int nMaxShuffle	// 配列サイズ
)
{
	for (int nCntShu = nMaxShuffle - 1; nCntShu > 0; nCntShu--)
	{ // 引数の総数分繰り返す

		T swap;	// 要素の入れ替え用
		int nRandom = rand() % nCntShu;	// 要素を入れ替えるインデックス

		// 要素の入れ替え
		swap					= pShuffleData[nRandom];
		pShuffleData[nRandom]	= pShuffleData[nCntShu];
		pShuffleData[nCntShu]	= swap;
	}
}

#endif	// _USEFUL_H_
