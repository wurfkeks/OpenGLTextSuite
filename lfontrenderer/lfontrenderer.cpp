/* ----------------------------------------------------------------------------
   Copyright (c) 2002, Lev Povalahev
   All rights reserved.

   Redistribution and use in source and binary forms, with or without modification,
   are permitted provided that the following conditions are met:

   * Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
   * Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
   * The name of the author may not be used to endorse or promote products
   derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
   ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
   WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
   IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
   INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
   BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
   OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
   ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
   THE POSSIBILITY OF SUCH DAMAGE.
   ------------------------------------------------------------------------------*/

#include "lfontrenderer.h"

#include <iostream>
#include <fstream>

// Function to determine endianness at run time
const int endian = 1;
#define is_BigEndian() ( (*(char*)&endian) == 0 )

// Swap little -> big endian
// This is needed for big endian architecures, as the font file is written in little endian.
// Also the font files assumes an int to be 32 bit, which might be a problem for 64 bit architectures.
#define SWAP_32(d) ((d >> 24) | ((d >> 8 ) & 0xff00) | ((d & 0xff00) << 8 ) | (d << 24))

//--------------------------------------------------
LFontRenderer::LFontRenderer()
{
    m_fonts.clear();
    m_strings.clear();
    m_activeFont = 0;
    m_strCount = 0;
#ifdef GL_VERSION_1_3
    m_textureUnits = 0;
#endif
    empty_string = "";
}


//--------------------------------------------------
LFontRenderer::~LFontRenderer()
{
    Clear();
}


//--------------------------------------------------
uint LFontRenderer::GetFontCount()
{
    return (uint)m_fonts.size();
}


//--------------------------------------------------
const std::string& LFontRenderer::GetFontName(uint index)
{
    if (index >= m_fonts.size())
    {
        printf( "LFontRenderer::GetFontName - index out of range\n"); fflush(stdout);
        return empty_string;
    }
    return m_fonts[index].name;
}


//--------------------------------------------------
void LFontRenderer::SetActiveFont(const std::string& name)
{
    for (uint i=0; i<m_fonts.size(); i++)
    {
        if (m_fonts[i].name == name)
        {
            m_activeFont = i;
            return;
        }
    }
    printf( "LFontRenderer::SetActiveFont - specified font not found\n"); fflush(stdout);
}


//--------------------------------------------------
void LFontRenderer::SetActiveFont(uint index)
{
    if (index >= m_fonts.size())
    {
        printf( "LFontRenderer::SetActiveFont - index out of range\n"); fflush(stdout);
        return;
    }
    m_activeFont = index;
}


//--------------------------------------------------
uint LFontRenderer::GetActiveFont()
{
    return m_activeFont;
}


//--------------------------------------------------
uint LFontRenderer::LoadFont(const std::string& fontname, const std::string& filename)
{
    LFont font;
    uint width;
    uint height;
    uint id;
    uint t;
    uint i;
    char *buf;

    // This union is needed to convert a float from little to big endian.
    // As SWAP_32 works only on int data types, the union combines both types.
    union {
        float			cvt_float;
        unsigned int	cvt_int;
    } swap;


    if ((filename == "") || (fontname == ""))
    {
        printf( "LFontRenderer::LoadFont - invalid filename or font name\n"); fflush(stdout);
        return 0;
    }

    std::ifstream file;
    file.open(filename.c_str(), std::ios::binary);
    if (!file.is_open())
    {
        printf( "LFontRenderer::LoadFont - error opening file\n"); fflush(stdout);
        return 0;
    }

    file.read((char*)&id, sizeof(id));
    if ( is_BigEndian() ) {
        id = SWAP_32(id);
    }

    if (id != 6666)
    {
        printf( "LFontRenderer::LoadFont - wrong file format id %u\n", id);
        fflush(stdout);
        return 0;
    }

    file.read((char*)&width, sizeof(uint));
    if ( is_BigEndian() ) {
        width = SWAP_32(width);
    }

    file.read((char*)&height, sizeof(uint));
    if ( is_BigEndian() ) {
        height = SWAP_32(height);
    }

    file.read((char*)&t, sizeof(uint));
    if ( is_BigEndian() ) {
        t = SWAP_32(t);
    }

    font.defaultHeight = t;
    for (i=0; i<256; i++)
    {
        file.read((char*)&t, sizeof(uint));
        if ( is_BigEndian() ) {
            t = SWAP_32(t);
        }

        font.chars[i].top = (float)t/(float)height;

        file.read((char*)&t, sizeof(uint));
        if ( is_BigEndian() ) {
            t = SWAP_32(t);
        }

        font.chars[i].left = (float)t/(float)width;

        file.read((char*)&t, sizeof(uint));
        if ( is_BigEndian() ) {
            t = SWAP_32(t);
        }

        font.chars[i].bottom = (float)t/(float)height;

        file.read((char*)&t, sizeof(uint));
        if ( is_BigEndian() ) {
            t = SWAP_32(t);
        }

        font.chars[i].right = (float)t/(float)width;

        file.read((char*)&t, sizeof(uint));
        if ( is_BigEndian() ) {
            t = SWAP_32(t);
        }

        font.chars[i].enabled = t != 0;

        file.read((char*)&swap.cvt_int, sizeof(float));
        if ( is_BigEndian() ) {
            swap.cvt_int = SWAP_32(swap.cvt_int);
        }

        font.chars[i].widthFactor = swap.cvt_float;

    }
    if ((file.eof()) || (file.fail()))
    {
        printf( "LFontRenderer::LoadFont - error reading from file\n"); fflush(stdout);
        return 0;
    }

    buf = (char*)malloc(width*height);
    if (buf == 0)
    {
        printf( "LFontRenderer::LoadFont - could not allocate memory\n"); fflush(stdout);
        return 0;
    }
    file.read(buf, width*height);
    if ((file.eof()) || (file.fail()))
    {
        free(buf);
        printf( "LFontRenderer::LoadFont - error reading from file\n"); fflush(stdout);
        return 0;
    }
    file.close();

    char *buf2 = (char*)malloc(width*height*2);
    for (i=0; i<width*height; i++)
    {
        buf2[i*2] = buf[i];
        buf2[i*2+1] = buf[i];
    }
    free(buf);

    glGenTextures(1, &font.textureId);
    glBindTexture(GL_TEXTURE_2D, font.textureId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, width, height, 0, GL_LUMINANCE_ALPHA, GL_UNSIGNED_BYTE, buf2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glBindTexture(GL_TEXTURE_2D, 0);

    free(buf2);

    font.r = 1;
    font.g = 1;
    font.b = 1;
    font.height = font.defaultHeight;
    font.name = fontname;
    font.widthScale = 1;
    font.angle = 0;
    font.italic = false;

    m_fonts.push_back(font);

    return m_fonts.size()-1;
}


//--------------------------------------------------
void LFontRenderer::Clear()
{
    for (uint i=0; i<m_fonts.size(); i++)
        glDeleteTextures(1, &m_fonts[i].textureId);
    m_fonts.clear();
    m_strings.clear();
    m_activeFont = 0;
    m_strCount = 0;
}


//--------------------------------------------------
void LFontRenderer::SetHeight(uint height)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::SetHeight - no valid active font selected\n"); fflush(stdout);
        return;
    }
    if (height == DEFAULT_HEIGHT)
        m_fonts[m_activeFont].height = m_fonts[m_activeFont].defaultHeight;
    else
        m_fonts[m_activeFont].height = height;
}


//--------------------------------------------------
void LFontRenderer::SetColor(GLfloat red, GLfloat green, GLfloat blue)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::SetColor - no valid active font selected\n"); fflush(stdout);
        return;
    }
    m_fonts[m_activeFont].r = red;
    m_fonts[m_activeFont].g = green;
    m_fonts[m_activeFont].b = blue;
}


//--------------------------------------------------
uint LFontRenderer::GetHeight() const
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::GetHeight - no valid active font selected\n"); fflush(stdout);
        return 0;
    }
    return m_fonts[m_activeFont].height;
}


//--------------------------------------------------
void LFontRenderer::GetColor(GLfloat &red, GLfloat &green, GLfloat &blue)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::GetColor - no valid active font selected\n"); fflush(stdout);
        return;
    }
    red = m_fonts[m_activeFont].r;
    green = m_fonts[m_activeFont].g;
    blue = m_fonts[m_activeFont].b;
}


//--------------------------------------------------
void LFontRenderer::StringOut(float x, float y, const std::string &string)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::StringOut - no valid active font selected\n"); fflush(stdout);
        return;
    }

    if (m_strCount >= m_strings.size())
        m_strings.resize(m_strings.size()+20);
    LStringData str;
    str.str = string;
    str.r = m_fonts[m_activeFont].r;
    str.g = m_fonts[m_activeFont].g;
    str.b = m_fonts[m_activeFont].b;
    str.widthScale = m_fonts[m_activeFont].widthScale;
    str.angle = m_fonts[m_activeFont].angle;
    str.font = m_activeFont;
    str.x = x;
    str.y = y;
    str.italic = m_fonts[m_activeFont].italic;
    str.height = m_fonts[m_activeFont].height;
    m_strings[m_strCount] = str;
    m_strCount++;
}


//--------------------------------------------------
void LFontRenderer::Draw()
{
    if (m_strCount == 0)
    {
        printf("LFontRenderer:Draw: no strings\n");
        fflush(stdout);
        return;
    }
#ifdef X86_64
#ifdef GL_VERSION_1_3
    if (m_textureUnits == 0) glGetIntegerv(GL_MAX_TEXTURE_UNITS, (GLint*)&m_textureUnits);

    for (uint i=0; i<m_textureUnits; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        // if you use 1d texturing or 3d texturing or cube map texturing you should disable it too
        if ( glIsEnabled(GL_TEXTURE_2D) ) {
            glDisable(GL_TEXTURE_2D);
        }
    }
    glActiveTexture(GL_TEXTURE0);
#endif
#endif
    glPolygonMode(GL_FRONT, GL_FILL);
//     glDisable(GL_CULL_FACE);
//     glDisable(GL_DEPTH_TEST);
//     glDisable(GL_LIGHTING);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glMatrixMode(GL_TEXTURE);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);

    glDisable(GL_TEXTURE_GEN_S);
    glDisable(GL_TEXTURE_GEN_T);
    glDisable(GL_ALPHA_TEST);
//     glEnable(GL_BLEND);
//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    byte c;
    uint cur_font;
    float next_x;
    float italic = 0;
    for (uint i=0; i<m_strCount; i++)
    {
        if (m_strings[i].font >= m_fonts.size())
        {
            printf( "LFontRenderer::Draw - invalid font index\n"); fflush(stdout);
            return;
        }

        cur_font = m_strings[i].font;
        glBindTexture(GL_TEXTURE_2D, m_fonts[cur_font].textureId);

        if (m_strings[i].italic)
            italic = m_fonts[cur_font].height*m_fonts[cur_font].widthScale * 0.3f;
        else
            italic = 0;

        double y1 = m_strings[i].y + m_strings[i].height - m_fonts[cur_font].height;
        double y2 = m_strings[i].y - m_fonts[cur_font].height;
        double y3 = m_strings[i].y + m_strings[i].height - m_fonts[cur_font].height;
        double y4 = m_strings[i].y - m_fonts[cur_font].height;

        next_x = 0;
        for (uint k=0; k<m_strings[i].str.length(); k++)
        {
            c = m_strings[i].str[k];
            if (!m_fonts[cur_font].chars[c].enabled) c = 32;

            glBegin(GL_TRIANGLE_STRIP);

            glTexCoord2f(m_fonts[cur_font].chars[c].left, m_fonts[cur_font].chars[c].bottom);
            glVertex2f(m_strings[i].x + next_x + italic,  y1);

            glTexCoord2f(m_fonts[cur_font].chars[c].left, m_fonts[cur_font].chars[c].top);
            glVertex2f(m_strings[i].x + next_x,           y2);

            glTexCoord2f(m_fonts[cur_font].chars[c].right,
                         m_fonts[cur_font].chars[c].bottom);
            glVertex2f(m_strings[i].x + next_x + m_strings[i].height*
                       m_strings[i].widthScale*m_fonts[cur_font].chars[c].widthFactor + italic,
                       y3);

            glTexCoord2f(m_fonts[cur_font].chars[c].right,
                         m_fonts[cur_font].chars[c].top);
            glVertex2f(m_strings[i].x + next_x + m_strings[i].height*
                       m_strings[i].widthScale*m_fonts[cur_font].chars[c].widthFactor,
                       y4);

//             glTexCoord2f(m_fonts[cur_font].chars[c].left,
//                          m_fonts[cur_font].chars[c].top);
//             glVertex2f(m_strings[i].x + next_x + italic,
//                        m_strings[i].y + m_strings[i].height);

//             glTexCoord2f(m_fonts[cur_font].chars[c].left,
//                          m_fonts[cur_font].chars[c].bottom);
//             glVertex2f(m_strings[i].x + next_x,
//                        m_strings[i].y);

//             glTexCoord2f(m_fonts[cur_font].chars[c].right,
//                          m_fonts[cur_font].chars[c].top);
//             glVertex2f(m_strings[i].x + next_x + m_strings[i].height*
//                        m_strings[i].widthScale*m_fonts[cur_font].chars[c].widthFactor + italic,
//                        m_strings[i].y + m_strings[i].height);

//             glTexCoord2f(m_fonts[cur_font].chars[c].right,
//                          m_fonts[cur_font].chars[c].bottom);
//             glVertex2f(m_strings[i].x + next_x + m_strings[i].height*
//                        m_strings[i].widthScale*m_fonts[cur_font].chars[c].widthFactor,
//                        m_strings[i].y);
            glEnd();

            next_x += m_strings[i].height*
                      m_strings[i].widthScale*
                      m_fonts[cur_font].chars[c].widthFactor;
        }
    }

    glDisable(GL_TEXTURE_2D);
    m_strCount = 0;
}

//--------------------------------------------------
void LFontRenderer::SetWidthScale(float scale)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::SetWidthScale - no valid active font selected\n"); fflush(stdout);
        return;
    }
    m_fonts[m_activeFont].widthScale = scale;
}


//--------------------------------------------------
float LFontRenderer::GetWidthScale()
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::GetWidthScale - no valid active font selected\n"); fflush(stdout);
        return 0.0;
    }
    return m_fonts[m_activeFont].widthScale;
}


//--------------------------------------------------
void LFontRenderer::SetRotation(float angle)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::SetRotation - no valid active font selected\n"); fflush(stdout);
        return;
    }

    m_fonts[m_activeFont].angle = angle;
}


//--------------------------------------------------
float LFontRenderer::GetRotation()
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::GetRotation - no valid active font selected\n"); fflush(stdout);
        return 0.0;
    }
    return m_fonts[m_activeFont].angle;
}


//--------------------------------------------------
void LFontRenderer::SetItalic(bool value)
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::SetItalic - no valid active font selected\n"); fflush(stdout);
        return;
    }
    m_fonts[m_activeFont].italic = value;
}


//--------------------------------------------------
bool LFontRenderer::GetItalic()
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::GetItalic - no valid active font selected\n"); fflush(stdout);
        return false;
    }
    return m_fonts[m_activeFont].italic;
}


//--------------------------------------------------
uint LFontRenderer::GetStringWidth(const std::string &string) const
{
    if (m_activeFont >= m_fonts.size())
    {
        printf( "LFontRenderer::GetStringWidth - no valid active font selected\n"); fflush(stdout);
        return 0;
    }
    float t = 0;
    for (uint i=0; i<string.length(); i++)
        if (m_fonts[m_activeFont].chars[(unsigned int)string[i]].enabled)
            t += m_fonts[m_activeFont].height*m_fonts[m_activeFont].chars[(unsigned int)string[i]].widthFactor*m_fonts[m_activeFont].widthScale;
        else
            t += m_fonts[m_activeFont].height*m_fonts[m_activeFont].chars[32].widthFactor*m_fonts[m_activeFont].widthScale;
    return (uint)t;
}
