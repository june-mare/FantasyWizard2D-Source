#include "Texture.h"
#include "Debug.h"
#include <filesystem>
#include <fstream>
#include <vector>

/*TGA�̃t�@�C���w�b�_*/
/*
���O			�I�t�Z�b�g		�o�C�g��		�i�[����Ă�����
ID Length		0				1				�C���[�WID�f�[�^�̃o�C�g
Colormap Type   1				1				�J���[�}�b�v�̗L�� 0 = ��
Image Type		2				1				�摜�L�^�`���B
												0�@= �摜�f�[�^�Ȃ�
												1�@= ���k�����A�C���f�b�N�X�E�J���[�摜
												2  = ���k�����A�J���[�摜
												3  = ���k�����A�����摜
												9  = ���k����C���f�b�N�X�J���[�摜
												10 = ���k����C���f�b�N�X�E�J���[�摜
												11 = ���k����A�����摜
First Entry Index 3				2				�摜�f�[�^�̃C���f�b�N�X�ɑΉ�����J���[
												�}�b�v�̃C���f�b�N�X�A�J���[�}�b�v�Ȃ��̏ꍇ�͖��g�p
Colormap Length 5				2				�J���[�}�b�v�ɓo�^����Ă���F�̐�
												�J���[�}�b�v���̏ꍇ��0(���g�p)
Colormap Entry Size	7			1				�J���[�}�b�v�ɓo�^����Ă���F�̈�̃r�b�g��
												�J���[�}�b�v�Ȃ��̏ꍇ��0
X-Origin		8				2				X���W�����炷�s�N�Z����
Y-Origin		10				2				Y���W�����炷�s�N�Z����
Image Width		12				2				�摜�̉��̃s�N�Z����
Image Height	14				2				�摜�̏c�̃s�N�Z����
Pixel Depth		16				1				�摜�̂P�s�N�Z���̃r�b�g��
Image Descriptor17				1				�摜�̊i�[�����y�тP�s�N�Z���̃A���t�@�v�f�̃r�b�g��
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
	/*�e�N�X�`�����쐬����*/
	file.open(filename, std::ios::binary);
	if (file)
	{
		const size_t tgaHeaderSize = 18;/*�w�b�_���̃o�C�g�T�C�Y*/
		/*�t�@�C����ǂݍ���*/
		
		file.read(reinterpret_cast<char*>(header), tgaHeaderSize);

		/*�w�b�_��񂩂�摜�̃T�C�Y�����o��*/
		width = header[12] + header[13] * 0x100;
		height = header[14] + header[15] * 0x100;

		/*�ǂݎ��惁�����̈���m��*/
		std::vector<char> img(
			std::filesystem::file_size(filename) - tgaHeaderSize);
		file.read(img.data(), img.size());

		/*�e�N�X�`�����쐬*/
		glCreateTextures(GL_TEXTURE_2D, 1, &tex);
		glTextureStorage2D(tex, 1, GL_RGBA8, width, height);
		glTextureSubImage2D(tex, 0, 0, 0, width, height,
			GL_BGRA, GL_UNSIGNED_BYTE, img.data());

		/*NEAREST�ɕύX*/
#if 1
			/*�^�C�����E���ňӐ}���Ȃ��F���\������Ȃ��悤�ɂ���*/
		glTextureParameteri(tex, GL_TEXTURE_MAG_FILTER, filterMode);
		glTextureParameteri(tex, GL_TEXTURE_MIN_FILTER, filterMode);

		/*�t�B���^�̓ǂݎ����@*/
		/*
		GL_LINEAR		�e�L�X�`�����W�̎��͂�4�e�N�Z���̉��d����
		GL_NEARREST		�e�L�X�`�����W�ɍł��߂�1�e�N�Z��
		*/
		/*�e�L�X�`���̃p�����[�^*/
		/*
		GL_TEXTURE_MAG_FILTER		�e�L�X�`���g�厞�̃t�B���^�ݒ�
		GL_TEXTURE_MIN_FILTER		�e�L�X�`���k�����̃t�B���^�ݒ�
		*/

#endif
	}
	else
	{
		LOG_ERROR("%s���J���܂���", filename.c_str());
	}
	file.close();
}

Texture::~Texture()
{
	glDeleteTextures(1 ,&tex);
}
