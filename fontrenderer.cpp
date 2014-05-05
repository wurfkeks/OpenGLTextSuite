#include "fontrenderer.h"

QMap<QString, FontRenderer*> FontRenderer::sFontRenderer;

FontRenderer::FontRenderer() {
}

FontRenderer* FontRenderer::getRenderer(const QString &renderer) {
    return sFontRenderer.value(renderer, NULL);
}

void FontRenderer::registerRenderer(FontRenderer *fr) {
    sFontRenderer.insert(fr->name(), fr);
}

QList<QString> FontRenderer::listRenderer() {
    return sFontRenderer.keys();
}
