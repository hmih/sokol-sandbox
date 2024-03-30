mod shader;

use std::ptr::addr_of_mut;

use sokol::{app as sapp, gfx as sg, glue as sglue};

struct State {
    pip: sg::Pipeline,
    bind: sg::Bindings,
}

static mut STATE: State = State {
    pip: sg::Pipeline::new(),
    bind: sg::Bindings::new(),
};

extern "C" fn init() {
    let state: &mut State = unsafe { &mut (*addr_of_mut!(STATE)) };

    sg::setup(&sg::Desc {
        environment: sglue::environment(),
        logger: sg::Logger {
            func: Some(sokol::log::slog_func),
            ..Default::default()
        },
        ..Default::default()
    });

    // triangle vertex buffer
    #[rustfmt::skip]
    const VERTICES: &[f32] = &[
        // position      color
        -0.5, -0.5, 0.0, 1.0, 0.0, 0.0, 1.0, // bot left
        -0.5,  0.5, 0.0, 0.0, 1.0, 0.0, 1.0, // top left
         0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0, // top right
         0.5, -0.5, 0.0, 1.0, 0.0, 1.0, 1.0, // bot right
    ];
    state.bind.vertex_buffers[0] = sg::make_buffer(&sg::BufferDesc {
        data: sg::slice_as_range(VERTICES),
        _type: sg::BufferType::Vertexbuffer,
        ..Default::default()
    });

    #[rustfmt::skip]
    const INDICES: &[u16] = &[
        0, 1, 3,
        1, 2, 3
    ];

    state.bind.index_buffer = sg::make_buffer(&sg::BufferDesc {
        data: sg::slice_as_range(INDICES),
        _type: sg::BufferType::Indexbuffer,
        ..Default::default()
    });

    // shader and pipeline object
    let be = sg::query_backend();
    let shader = &shader::simple_shader_desc(be);
    state.pip = sg::make_pipeline(&sg::PipelineDesc {
        shader: sg::make_shader(shader),
        index_type: sg::IndexType::Uint16,
        layout: {
            let mut layout = sg::VertexLayoutState::new();
            layout.attrs[shader::ATTR_VS_POS0].format = sg::VertexFormat::Float3;
            layout.attrs[shader::ATTR_VS_COL0].format = sg::VertexFormat::Float4;
            layout
        },
        ..Default::default()
    });
}

extern "C" fn frame() {
    let state: &mut State = unsafe { &mut (*addr_of_mut!(STATE)) };

    let mut pass_action = sg::PassAction::new();
    pass_action.colors[0] = sg::ColorAttachmentAction {
        load_action: sg::LoadAction::Clear,
        clear_value: sg::Color {
            r: 0.0,
            g: 0.0,
            b: 0.0,
            a: 0.0,
        },
        ..Default::default()
    };

    sg::begin_pass(&sg::Pass {
        action: pass_action,
        swapchain: sglue::swapchain(),
        ..Default::default()
    });
    sg::apply_pipeline(state.pip);
    sg::apply_bindings(&state.bind);
    sg::draw(0, 36, 1);
    sg::end_pass();
    sg::commit();
}

extern "C" fn cleanup() {
    sg::shutdown()
}

fn main() {
    sapp::run(&sapp::Desc {
        init_cb: Some(init),
        frame_cb: Some(frame),
        cleanup_cb: Some(cleanup),
        width: 800,
        height: 600,
        sample_count: 4,
        window_title: c"simple".as_ptr(),
        logger: sapp::Logger {
            func: Some(sokol::log::slog_func),
            ..Default::default()
        },
        icon: sapp::IconDesc {
            sokol_default: true,
            ..Default::default()
        },

        ..Default::default()
    });
}
