/*
 * come from https://github.com/skywind3000/RenderHelp
 */

#pragma once

#include <cstring>
#include <string>

#include "math.h"

class Bitmap {
   public:
    inline virtual ~Bitmap() {
        if (m_bits) delete[] m_bits;
        m_bits = NULL;
    }
    inline Bitmap(int width, int height) : m_width(width), m_height(height) {
        m_pitch = width * 4;
        m_bits = new uint8_t[m_pitch * m_height];
        Fill(0);
    }

    inline Bitmap(const Bitmap &src)
        : m_width(src.m_width), m_height(src.m_height), m_pitch(src.m_pitch) {
        m_bits = new uint8_t[m_pitch * m_height];
        memcpy(m_bits, src.m_bits, m_pitch * m_height);
    }

    inline Bitmap(const char *filename) {
        Bitmap *tmp = LoadFile(filename);
        if (tmp == NULL) {
            std::string msg = "load failed: ";
            msg.append(filename);
            throw std::runtime_error(msg);
        }
        m_width = tmp->m_width;
        m_height = tmp->m_height;
        m_pitch = tmp->m_pitch;
        m_bits = tmp->m_bits;
        tmp->m_bits = NULL;
        delete tmp;
    }

   public:
    inline int GetW() const { return m_width; }
    inline int GetH() const { return m_height; }
    inline int GetPitch() const { return m_pitch; }
    inline uint8_t *GetBits() { return m_bits; }
    inline const uint8_t *GetBits() const { return m_bits; }
    inline uint8_t *GetLine(int y) { return m_bits + m_pitch * y; }
    inline const uint8_t *GetLine(int y) const { return m_bits + m_pitch * y; }

   public:
    inline void Fill(uint32_t color) {
        for (int j = 0; j < m_height; j++) {
            uint32_t *row = (uint32_t *)(m_bits + j * m_pitch);
            for (int i = 0; i < m_width; i++, row++)
                memcpy(row, &color, sizeof(uint32_t));
        }
    }

    inline void SetPixel(int x, int y, uint32_t color) {
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            memcpy(m_bits + y * m_pitch + x * 4, &color, sizeof(uint32_t));
        }
    }

    inline uint32_t GetPixel(int x, int y) const {
        uint32_t color = 0;
        if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
            memcpy(&color, m_bits + y * m_pitch + x * 4, sizeof(uint32_t));
        }
        return color;
    }

    inline void DrawLine(int x1, int y1, int x2, int y2, uint32_t color) {
        int x, y;
        if (x1 == x2 && y1 == y2) {
            SetPixel(x1, y1, color);
            return;
        } else if (x1 == x2) {
            int inc = (y1 <= y2) ? 1 : -1;
            for (y = y1; y != y2; y += inc) SetPixel(x1, y, color);
            SetPixel(x2, y2, color);
        } else if (y1 == y2) {
            int inc = (x1 <= x2) ? 1 : -1;
            for (x = x1; x != x2; x += inc) SetPixel(x, y1, color);
            SetPixel(x2, y2, color);
        } else {
            int dx = (x1 < x2) ? x2 - x1 : x1 - x2;
            int dy = (y1 < y2) ? y2 - y1 : y1 - y2;
            int rem = 0;
            if (dx >= dy) {
                if (x2 < x1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
                for (x = x1, y = y1; x <= x2; x++) {
                    SetPixel(x, y, color);
                    rem += dy;
                    if (rem >= dx) {
                        rem -= dx;
                        y += (y2 >= y1) ? 1 : -1;
                        SetPixel(x, y, color);
                    }
                }
                SetPixel(x2, y2, color);
            } else {
                if (y2 < y1) x = x1, y = y1, x1 = x2, y1 = y2, x2 = x, y2 = y;
                for (x = x1, y = y1; y <= y2; y++) {
                    SetPixel(x, y, color);
                    rem += dx;
                    if (rem >= dy) {
                        rem -= dy;
                        x += (x2 >= x1) ? 1 : -1;
                        SetPixel(x, y, color);
                    }
                }
                SetPixel(x2, y2, color);
            }
        }
    }

    struct BITMAPINFOHEADER {  // bmih
        uint32_t biSize;
        uint32_t biWidth;
        int32_t biHeight;
        uint16_t biPlanes;
        uint16_t biBitCount;
        uint32_t biCompression;
        uint32_t biSizeImage;
        uint32_t biXPelsPerMeter;
        uint32_t biYPelsPerMeter;
        uint32_t biClrUsed;
        uint32_t biClrImportant;
    };

    // 读取 BMP 图片，支持 24/32 位两种格式
    inline static Bitmap *LoadFile(const char *filename) {
        FILE *fp = fopen(filename, "rb");
        if (fp == NULL) return NULL;
        BITMAPINFOHEADER info;
        uint8_t header[14];
        int hr = (int)fread(header, 1, 14, fp);
        if (hr != 14) {
            fclose(fp);
            return NULL;
        }
        if (header[0] != 0x42 || header[1] != 0x4d) {
            fclose(fp);
            return NULL;
        }
        hr = (int)fread(&info, 1, sizeof(info), fp);
        if (hr != 40) {
            fclose(fp);
            return NULL;
        }
        if (info.biBitCount != 24 && info.biBitCount != 32) {
            fclose(fp);
            return NULL;
        }
        Bitmap *bmp = new Bitmap(info.biWidth, info.biHeight);
        uint32_t offset;
        memcpy(&offset, header + 10, sizeof(uint32_t));
        fseek(fp, offset, SEEK_SET);
        uint32_t pixelsize = (info.biBitCount + 7) / 8;
        uint32_t pitch = (pixelsize * info.biWidth + 3) & (~3);
        for (int y = 0; y < (int)info.biHeight; y++) {
            uint8_t *line = bmp->GetLine(info.biHeight - 1 - y);
            for (int x = 0; x < (int)info.biWidth; x++, line += 4) {
                line[3] = 255;
                fread(line, pixelsize, 1, fp);
            }
            fseek(fp, pitch - info.biWidth * pixelsize, SEEK_CUR);
        }
        fclose(fp);
        return bmp;
    }

    // 保存 BMP 图片
    inline bool SaveFile(const char *filename, bool withAlpha = false) const {
        FILE *fp = fopen(filename, "wb");
        if (fp == NULL) return false;
        BITMAPINFOHEADER info;
        uint32_t pixelsize = (withAlpha) ? 4 : 3;
        uint32_t pitch = (GetW() * pixelsize + 3) & (~3);
        info.biSizeImage = pitch * GetH();
        uint32_t bfSize = 54 + info.biSizeImage;
        uint32_t zero = 0, offset = 54;
        fputc(0x42, fp);
        fputc(0x4d, fp);
        fwrite(&bfSize, 4, 1, fp);
        fwrite(&zero, 4, 1, fp);
        fwrite(&offset, 4, 1, fp);
        info.biSize = 40;
        info.biWidth = GetW();
        info.biHeight = GetH();
        info.biPlanes = 1;
        info.biBitCount = (withAlpha) ? 32 : 24;
        info.biCompression = 0;
        info.biXPelsPerMeter = 0xb12;
        info.biYPelsPerMeter = 0xb12;
        info.biClrUsed = 0;
        info.biClrImportant = 0;
        fwrite(&info, sizeof(info), 1, fp);
        // printf("pitch=%d %d\n", (int)pitch, info.biSizeImage);
        for (int y = 0; y < GetH(); y++) {
            const uint8_t *line = GetLine(info.biHeight - 1 - y);
            uint32_t padding = pitch - GetW() * pixelsize;
            for (int x = 0; x < GetW(); x++, line += 4) {
                fwrite(line, pixelsize, 1, fp);
            }
            for (int i = 0; i < (int)padding; i++) fputc(0, fp);
        }
        fclose(fp);
        return true;
    }

    // 双线性插值
    inline uint32_t SampleBilinear(float x, float y) const {
        int32_t fx = (int32_t)(x * 0x10000);
        int32_t fy = (int32_t)(y * 0x10000);
        int32_t x1 = Between(0, m_width - 1, fx >> 16);
        int32_t y1 = Between(0, m_height - 1, fy >> 16);
        int32_t x2 = Between(0, m_width - 1, x1 + 1);
        int32_t y2 = Between(0, m_height - 1, y1 + 1);
        int32_t dx = (fx >> 8) & 0xff;
        int32_t dy = (fy >> 8) & 0xff;
        if (m_width <= 0 || m_height <= 0) return 0;
        uint32_t c00 = GetPixel(x1, y1);
        uint32_t c01 = GetPixel(x2, y1);
        uint32_t c10 = GetPixel(x1, y2);
        uint32_t c11 = GetPixel(x2, y2);
        return BilinearInterp(c00, c01, c10, c11, dx, dy);
    }

    // 纹理采样
    inline Vec4f Sample2D(float u, float v) const {
        uint32_t rgba = SampleBilinear(u * m_width + 0.5f, v * m_height + 0.5f);
        return vector_from_color(rgba);
    }

    // 纹理采样：直接传入 Vec2f
    inline Vec4f Sample2D(const Vec2f &uv) const {
        return Sample2D(uv.x, uv.y);
    }

    // 按照 Vec4f 画点
    inline void SetPixel(int x, int y, const Vec4f &color) {
        SetPixel(x, y, vector_to_color(color));
    }

    // 上下反转
    inline void FlipVertical() {
        uint8_t *buffer = new uint8_t[m_pitch];
        for (int i = 0, j = m_height - 1; i < j; i++, j--) {
            memcpy(buffer, GetLine(i), m_pitch);
            memcpy(GetLine(i), GetLine(j), m_pitch);
            memcpy(GetLine(j), buffer, m_pitch);
        }
        delete[] buffer;
    }

    // 水平反转
    inline void FlipHorizontal() {
        for (int y = 0; y < m_height; y++) {
            for (int i = 0, j = m_width - 1; i < j; i++, j--) {
                uint32_t c1 = GetPixel(i, y);
                uint32_t c2 = GetPixel(j, y);
                SetPixel(i, y, c2);
                SetPixel(j, y, c1);
            }
        }
    }

   protected:
    // 双线性插值计算：给出四个点的颜色，以及坐标偏移，计算结果
    inline static uint32_t BilinearInterp(uint32_t tl, uint32_t tr, uint32_t bl,
                                          uint32_t br, int32_t distx,
                                          int32_t disty) {
        uint32_t f, r;
        int32_t distxy = distx * disty;
        int32_t distxiy = (distx << 8) - distxy; /* distx * (256 - disty) */
        int32_t distixy = (disty << 8) - distxy; /* disty * (256 - distx) */
        int32_t distixiy = 256 * 256 - (disty << 8) - (distx << 8) + distxy;
        r = (tl & 0x000000ff) * distixiy + (tr & 0x000000ff) * distxiy +
            (bl & 0x000000ff) * distixy + (br & 0x000000ff) * distxy;
        f = (tl & 0x0000ff00) * distixiy + (tr & 0x0000ff00) * distxiy +
            (bl & 0x0000ff00) * distixy + (br & 0x0000ff00) * distxy;
        r |= f & 0xff000000;
        tl >>= 16;
        tr >>= 16;
        bl >>= 16;
        br >>= 16;
        r >>= 16;
        f = (tl & 0x000000ff) * distixiy + (tr & 0x000000ff) * distxiy +
            (bl & 0x000000ff) * distixy + (br & 0x000000ff) * distxy;
        r |= f & 0x00ff0000;
        f = (tl & 0x0000ff00) * distixiy + (tr & 0x0000ff00) * distxiy +
            (bl & 0x0000ff00) * distixy + (br & 0x0000ff00) * distxy;
        r |= f & 0xff000000;
        return r;
    }

   protected:
    int32_t m_width;
    int32_t m_height;
    int32_t m_pitch;
    uint8_t *m_bits;
};