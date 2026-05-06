<template>
  <div id="app">
    <!-- ── Nav ─────────────────────────────────────────────────────────── -->
    <nav class="navbar">
      <div class="container nav-inner">
        <!-- Brand -->
        <RouterLink to="/jobs" class="nav-brand">
          <span class="brand-icon">
            <svg width="22" height="22" viewBox="0 0 24 24" fill="none">
              <path d="M20 7H4C2.9 7 2 7.9 2 9v12c0 1.1.9 2 2 2h16c1.1 0 2-.9 2-2V9c0-1.1-.9-2-2-2z"
                stroke="var(--c-accent)" stroke-width="1.8" fill="none"/>
              <path d="M16 7V5c0-1.1-.9-2-2-2h-4c-1.1 0-2 .9-2 2v2" stroke="var(--c-accent)" stroke-width="1.8" fill="none"/>
              <line x1="12" y1="12" x2="12" y2="17" stroke="var(--c-accent)" stroke-width="1.8" stroke-linecap="round"/>
              <line x1="9" y1="14.5" x2="15" y2="14.5" stroke="var(--c-accent)" stroke-width="1.8" stroke-linecap="round"/>
            </svg>
          </span>
          <span class="brand-name">SJVMS</span>
        </RouterLink>

        <!-- Links -->
        <div class="nav-links">
          <RouterLink to="/jobs" class="nav-link">Browse Jobs</RouterLink>
          <template v-if="auth.isLoggedIn">
            <RouterLink to="/dashboard" class="nav-link">Dashboard</RouterLink>
            <RouterLink v-if="auth.user?.role === 'employer'" to="/manage-jobs" class="nav-link">My Jobs</RouterLink>
          </template>
        </div>

        <!-- Auth -->
        <div class="nav-auth">
          <template v-if="!auth.isLoggedIn">
            <RouterLink to="/login" class="btn btn-ghost btn-sm">Sign In</RouterLink>
            <RouterLink to="/register" class="btn btn-primary btn-sm">Get Started</RouterLink>
          </template>
          <template v-else>
            <div class="nav-user" @click="menuOpen = !menuOpen" ref="menuRef">
              <div class="avatar">{{ initials }}</div>
              <span class="nav-username">{{ auth.user?.name }}</span>
              <svg width="14" height="14" viewBox="0 0 24 24" fill="none" :style="{ transform: menuOpen ? 'rotate(180deg)' : '', transition: 'transform .2s' }">
                <path d="M6 9l6 6 6-6" stroke="currentColor" stroke-width="2" stroke-linecap="round"/>
              </svg>
            </div>
            <!-- Dropdown -->
            <Transition name="fade">
              <div v-if="menuOpen" class="nav-dropdown">
                <div class="dropdown-header">
                  <strong>{{ auth.user?.name }}</strong>
                  <span class="badge badge-purple">{{ auth.user?.role }}</span>
                </div>
                <div class="dropdown-divider"/>
                <RouterLink to="/profile" class="dropdown-item" @click="menuOpen = false">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><circle cx="12" cy="8" r="4" stroke="currentColor" stroke-width="1.8"/><path d="M4 20c0-4 3.6-7 8-7s8 3 8 7" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>
                  Profile
                </RouterLink>
                <RouterLink to="/applications" class="dropdown-item" @click="menuOpen = false">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><rect x="3" y="3" width="18" height="18" rx="2" stroke="currentColor" stroke-width="1.8"/><path d="M7 8h10M7 12h10M7 16h6" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>
                  Applications
                </RouterLink>
                <template v-if="auth.user?.role === 'employer'">
                  <RouterLink to="/post-job" class="dropdown-item" @click="menuOpen = false">
                    <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><circle cx="12" cy="12" r="9" stroke="currentColor" stroke-width="1.8"/><path d="M12 8v8M8 12h8" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>
                    Post a Job
                  </RouterLink>
                </template>
                <div class="dropdown-divider"/>
                <button class="dropdown-item danger" @click="handleLogout">
                  <svg width="15" height="15" viewBox="0 0 24 24" fill="none"><path d="M17 16l4-4-4-4M21 12H9M13 21H5a2 2 0 01-2-2V5a2 2 0 012-2h8" stroke="currentColor" stroke-width="1.8" stroke-linecap="round"/></svg>
                  Sign Out
                </button>
              </div>
            </Transition>
          </template>
        </div>
      </div>
    </nav>

    <!-- ── Page ────────────────────────────────────────────────────────── -->
    <RouterView v-slot="{ Component }">
      <Transition name="fade" mode="out-in">
        <component :is="Component" />
      </Transition>
    </RouterView>
  </div>
</template>

<script setup lang="ts">
import { ref, computed, onMounted, onUnmounted } from 'vue'
import { useRouter } from 'vue-router'
import { useAuthStore } from './stores/auth'
import { useDataStore } from './stores/data'

const auth = useAuthStore()
const data = useDataStore()
const router = useRouter()

const menuOpen = ref(false)
const menuRef = ref<HTMLElement | null>(null)

// FIXED: Removed .value and used auth.user directly
const initials = computed(() => {
  const name = auth.user?.name || ''
  return name.split(' ').map((w: string) => w[0]).join('').toUpperCase().slice(0, 2)
})

function handleLogout() {
  menuOpen.value = false
  auth.logout()
  router.push('/login')
}

function handleClickOutside(e: MouseEvent) {
  if (menuRef.value && !menuRef.value.contains(e.target as Node)) {
    menuOpen.value = false
  }
}

onMounted(async () => {
  auth.restoreSession()
  await data.loadAll()
  document.addEventListener('click', handleClickOutside)
})

onUnmounted(() => {
  document.removeEventListener('click', handleClickOutside)
})
</script>

<style scoped>
/* ── Navbar ──────────────────────────────────────────────────────────────── */
.navbar {
  position: fixed;
  top: 0;
  left: 0;
  right: 0;
  z-index: 100;
  height: var(--nav-h);
  background: rgba(13,15,20,.85);
  backdrop-filter: blur(16px);
  border-bottom: 1px solid var(--c-border);
}

.nav-inner {
  height: 100%;
  display: flex;
  align-items: center;
  gap: 32px;
}

.nav-brand {
  display: flex;
  align-items: center;
  gap: 10px;
  flex-shrink: 0;
}
.brand-icon {
  width: 36px;
  height: 36px;
  background: rgba(79,142,247,.1);
  border: 1px solid rgba(79,142,247,.2);
  border-radius: 9px;
  display: grid;
  place-items: center;
}
.brand-name {
  font-family: var(--font-display);
  font-size: 1.1rem;
  font-weight: 800;
  letter-spacing: .04em;
  background: linear-gradient(135deg, var(--c-accent), var(--c-accent2));
  -webkit-background-clip: text;
  background-clip: text;
  -webkit-text-fill-color: transparent;
}

.nav-links {
  display: flex;
  align-items: center;
  gap: 4px;
  flex: 1;
}
.nav-link {
  padding: 6px 14px;
  border-radius: var(--radius-sm);
  font-size: .88rem;
  font-weight: 500;
  color: var(--c-text2);
  transition: color .15s, background .15s;
}
.nav-link:hover { color: var(--c-text); background: var(--c-surface2); }
.nav-link.router-link-active { color: var(--c-text); background: var(--c-surface2); }

.nav-auth { display: flex; align-items: center; gap: 8px; margin-left: auto; }

.nav-user {
  display: flex;
  align-items: center;
  gap: 8px;
  padding: 6px 12px 6px 6px;
  border-radius: var(--radius-sm);
  border: 1px solid var(--c-border);
  cursor: pointer;
  transition: border-color .15s, background .15s;
  position: relative;
}
.nav-user:hover { border-color: var(--c-border2); background: var(--c-surface2); }

.avatar {
  width: 28px;
  height: 28px;
  border-radius: 50%;
  background: linear-gradient(135deg, var(--c-accent), var(--c-accent2));
  display: grid;
  place-items: center;
  font-size: .7rem;
  font-weight: 700;
  color: #fff;
  flex-shrink: 0;
}
.nav-username { font-size: .85rem; font-weight: 500; max-width: 120px; overflow: hidden; text-overflow: ellipsis; white-space: nowrap; }

/* Dropdown */
.nav-dropdown {
  position: absolute;
  top: calc(100% + 8px);
  right: 0;
  width: 220px;
  background: var(--c-surface);
  border: 1px solid var(--c-border2);
  border-radius: var(--radius-md);
  box-shadow: var(--shadow-lg);
  overflow: hidden;
  z-index: 200;
}
.dropdown-header {
  padding: 12px 16px;
  display: flex;
  align-items: center;
  justify-content: space-between;
  font-size: .85rem;
}
.dropdown-divider { height: 1px; background: var(--c-border); }
.dropdown-item {
  display: flex;
  align-items: center;
  gap: 10px;
  padding: 10px 16px;
  font-size: .88rem;
  color: var(--c-text2);
  width: 100%;
  background: none;
  border: none;
  cursor: pointer;
  transition: background .15s, color .15s;
  text-align: left;
}
.dropdown-item:hover { background: var(--c-surface2); color: var(--c-text); }
.dropdown-item.danger:hover { color: var(--c-red); }
</style>