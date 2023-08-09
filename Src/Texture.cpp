#include "Texture.h"
#include "Debug.h"
#include <filesystem>
#include <fstream>
#include <vector>

/*TGAのファイルヘッダ*/
/*
名前			オフセット		バイト数		格納されている情報
ID Length		0				1				イメージIDデータのバイト
Colormap Type   1				1				カラーマップの有無 0 = 無
Image Type		2				1				画像記録形式。
												0　= 画像データなし
												1　= 圧縮無し、インデックス・カラー画像
												2  = 圧縮無し、カラー画像
												3  = 圧縮無し、白黒画像
												9  = 圧縮ありインデックスカラー画像
												10 = 圧縮ありインデックス・カラー画像
												11 = 圧縮あり、白黒画像
First Entry Index 3				2				画像データのインデックスに対応するカラー
												マップのインデックス、カラーマップなしの場合は未使用
Colormap Length 5				2				カラーマップに登録されている色の数
												カラーマップ内の場合は0(未使用)
Colormap Entry Size	7			1				カラーマップに登録されている色の一つのビット数
												カラーマップなしの場合は0
X-Origin		8				2				X座標をずらすピクセル数
Y-Origin		10				2				Y座標をずらすピクセル数
Image Width		12				2				画像の横のピクセル数
Image Height	14				2				画像の縦のピクセル数
Pixel Depth		16				1				画像の１ピクセルのビット数
Image Descriptor17				1				画像の格納方向及び１ピクセルのアルファ要素のビット数
*/

TexturePtr Texture::Create(const std::string& filename, GLenum filterMode)
{
	return std::make_shared<Texture>(filename,filterMode);
}

Texture::Texture(const std::string& filename, GLenum filterMode)
	:filename(filename)
{
	std::ifstream file;
	//width = 0; height = 0;
	/*テクスチャを作成する*/
	file.open(filename, std::ios::binary);
	if (file)
	{
		const size_t tgaHeaderSize = 18;/*ヘッダ情報のバイトサイズ*/
		/*ファイルを読み込む*/
		
		file.read(reinterpret_cast<char*>(header), tgaHeaderSize);

		/*ヘッダ情報から画像のサイズを取り出す*/
		width = header[12] + header[13] * 0x100;
		height = header[14] + header[15] * 0x100;

		/*読み取り先メモリ領域を確保*/
		std::vector<char> img(
			std::filesystem::file_size(filename) - tgaHeaderSize);
		file.read(img.data(), img.size());

		/*テクスチャを作成*/
		glCreateTextures(GL_TEXTURE_2D, 1, &tex);
		glTextureStorage2D(tex, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(tex, 0, 0, 0, width, height,
			GL_BGRA, GL_UNSIGNED_BYTE, img.data());

		/*NEARESTに変更*/
#if 1
			/*タイル境界線で意図しない色が表示されないようにする*/
		glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMode);
		glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMode);

		/*フィルタの読み取り方法*/
		/*
		GL_LINEAR		テキスチャ座標の周囲の4テクセルの加重平均
		GL_NEARREST		テキスチャ座標に最も近い1テクセル
		*/
		/*テキスチャのパラメータ*/
		/*
		GL_TEXTURE_MAG_FILTER		テキスチャ拡大時のフィルタ設定
		GL_TEXTURE_MIN_FILTER		テキスチャ縮小時のフィルタ設定
		*/

#endif
	}
	else
	{
		LOG_ERROR("%sを開けません", filename.c_str());
	}
	file.close();
}

Texture::~Texture()
{
	glDeleteTextures(1 ,&tex);
}
