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

const PLAYER: isize = 0;
const COMPUTER: isize = 1;
const ENTITIES_SIZE: usize = 2;

const BOX_WIDTH: f32 = 0.15;
const BOX_HEIGHT: f32 = 0.15;

struct Entities {
    x: [f32; ENTITIES_SIZE],
    y: [f32; ENTITIES_SIZE],
    c: [u32; ENTITIES_SIZE],
}

static mut PLAYERS: Entities = Entities {
    x: [-0.15, 0.15],
    y: [0.05, -0.95],
    c: [0x00FF00FF, 0xFF0000FF],
};

#[derive(Debug)]
pub struct Vertex {
    pub x: f32,
    pub y: f32,
    pub c: u32,
}

fn to_indices() -> Vec<u32> {
    let mut res = Vec::with_capacity(ENTITIES_SIZE);

    for i in 0..ENTITIES_SIZE {
        // top left
        // top right
        // bot left
        //
        // top right
        // bot left
        // bot right
        let tl = (i * ENTITIES_SIZE) as u32;
        let ixs = [tl, tl + 1, tl + 2, tl + 1, tl + 2, tl + 3];
        res.extend(ixs);
    }

    res
}

fn to_vertices(es: &Entities) -> Vec<Vec<Vertex>> {
    let mut res = Vec::with_capacity(ENTITIES_SIZE);

    for i in 0..ENTITIES_SIZE {
        // top left
        // top right
        // bot left
        // bot right
        #[rustfmt::skip]
        let v = vec![
            Vertex{ x: es.x[i],             y: es.y[i],               c: es.c[i] },
            Vertex{ x: es.x[i] + BOX_WIDTH, y: es.y[i],               c: es.c[i] },
            Vertex{ x: es.x[i],             y: es.y[i] - BOX_HEIGHT,  c: es.c[i] },
            Vertex{ x: es.x[i] + BOX_WIDTH, y: es.y[i] - BOX_HEIGHT,  c: es.c[i] },
        ];
        res.push(v);
    }

    res
}

extern "C" fn init() {
    let state: &mut State = unsafe { &mut (*addr_of_mut!(STATE)) };
    let players: &mut Entities = unsafe { &mut (*addr_of_mut!(PLAYERS)) };

    sg::setup(&sg::Desc {
        environment: sglue::environment(),
        logger: sg::Logger {
            func: Some(sokol::log::slog_func),
            ..Default::default()
        },
        ..Default::default()
    });

    let vertices = to_vertices(players);

    for (ix, vs) in vertices.iter().enumerate() {
        state.bind.vertex_buffers[ix] = sg::make_buffer(&sg::BufferDesc {
            data: sg::slice_as_range(vs.as_slice()),
            _type: sg::BufferType::Vertexbuffer,
            ..Default::default()
        });
    }

    let indices = to_indices();
    state.bind.index_buffer = sg::make_buffer(&sg::BufferDesc {
        data: sg::slice_as_range(indices.as_slice()),
        _type: sg::BufferType::Indexbuffer,
        ..Default::default()
    });

    // shader and pipeline object
    let be = sg::query_backend();
    let shader = &shader::simple_shader_desc(be);
    state.pip = sg::make_pipeline(&sg::PipelineDesc {
        shader: sg::make_shader(shader),
        index_type: sg::IndexType::Uint32,
        layout: {
            let mut layout = sg::VertexLayoutState::new();
            layout.attrs[shader::ATTR_VS_POS0].format = sg::VertexFormat::Float3;
            layout.attrs[shader::ATTR_VS_COL0].format = sg::VertexFormat::Ubyte4n;
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
            r: 1.0,
            g: 1.0,
            b: 1.0,
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
