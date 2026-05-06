<script setup lang="ts">
import { ref, reactive, computed } from 'vue'
import { useAuthStore } from '../stores/auth'
import { useDataStore } from '../stores/data'
import SkillsInput from '../components/SkillsInput.vue'
import type { User } from '../types' 

const auth = useAuthStore()
const data = useDataStore()

const form = reactive({
  name:     auth.user?.name ?? '',
  username: auth.user?.username ?? '',
  email:    auth.user?.email ?? '',
  phone:    auth.user?.phone ?? '',
  skills:   [...(auth.user?.skills ?? [])],
  password: '',
})

const saved = ref(false)

const initials = computed(() =>
  form.name.split(' ').map(w => w[0]).join('').toUpperCase().slice(0, 2),
)

const myApps = computed(() => auth.user ? data.getApplicationsByUser(auth.user.id) : [])
const totalApps    = computed(() => myApps.value.length)
const pendingApps  = computed(() => myApps.value.filter(a => a.status === 'pending').length)
const approvedApps = computed(() => myApps.value.filter(a => a.status === 'approved').length)
const rejectedApps = computed(() => myApps.value.filter(a => a.status === 'rejected').length)

function handleSave() {
  const currentUser = auth.user  // local const lets TypeScript narrow correctly
  if (!currentUser) return
  const updated: User = {
    ...currentUser,
    name: form.name, username: form.username,
    email: form.email, phone: form.phone,
    skills: form.skills,
    password: form.password || currentUser.password,
  }
  data.updateUser(updated)
  auth.login(updated)
  form.password = ''
  saved.value = true
  setTimeout(() => (saved.value = false), 3000)
}
</script>
<template>
  <div class="page">
    <div class="container" style="max-width:680px">
      <div class="page-heading">
        <h1>My Profile</h1>
        <p>Update your personal information</p>
      </div>

      <Transition name="fade">
        <div v-if="saved" class="alert alert-success">Profile updated successfully!</div>
      </Transition>

      <div class="profile-card card">
        <!-- Avatar row -->
        <div class="profile-top">
          <div class="profile-avatar">{{ initials }}</div>
          <div>
            <h2 class="profile-name">{{ form.name }}</h2>
            <div class="profile-tags">
              <span class="badge badge-purple">{{ auth.user?.role }}</span>
              <span class="badge badge-gray">ID #{{ auth.user?.id }}</span>
            </div>
          </div>
        </div>
        <div class="divider"/>

        <form class="profile-form" @submit.prevent="handleSave">
          <div class="form-row">
            <div class="form-group">
              <label class="form-label">Full Name</label>
              <input v-model="form.name" class="form-input" required />
            </div>
            <div class="form-group">
              <label class="form-label">Username</label>
              <input v-model="form.username" class="form-input" required />
            </div>
          </div>
          <div class="form-group">
            <label class="form-label">Email</label>
            <input v-model="form.email" class="form-input" type="email" required />
          </div>
          <div class="form-group">
            <label class="form-label">Phone</label>
            <input v-model="form.phone" class="form-input" placeholder="07X XXXXXXX" />
          </div>
          <div v-if="auth.user?.role === 'applicant'" class="form-group">
            <label class="form-label">Skills</label>
            <SkillsInput v-model="form.skills" placeholder="Add your skills…" />
            <p class="form-hint">These are used to match you with relevant jobs.</p>
          </div>
          <div class="divider"/>
          <div class="form-group">
            <label class="form-label">New Password (leave blank to keep current)</label>
            <input v-model="form.password" class="form-input" type="password" placeholder="••••••••" autocomplete="new-password" />
          </div>
          <div class="form-footer">
            <button type="submit" class="btn btn-primary btn-lg">Save Changes</button>
          </div>
        </form>
      </div>

      <!-- Stats card for applicants -->
      <div v-if="auth.user?.role === 'applicant'" class="stats-mini card">
        <h3>Application Stats</h3>
        <div class="stats-mini-row">
          <div class="stat-mini-item">
            <span class="stat-mini-val">{{ totalApps }}</span>
            <span class="stat-mini-lbl">Total</span>
          </div>
          <div class="stat-mini-item">
            <span class="stat-mini-val" style="color:var(--c-amber)">{{ pendingApps }}</span>
            <span class="stat-mini-lbl">Pending</span>
          </div>
          <div class="stat-mini-item">
            <span class="stat-mini-val" style="color:var(--c-green)">{{ approvedApps }}</span>
            <span class="stat-mini-lbl">Approved</span>
          </div>
          <div class="stat-mini-item">
            <span class="stat-mini-val" style="color:var(--c-red)">{{ rejectedApps }}</span>
            <span class="stat-mini-lbl">Rejected</span>
          </div>
        </div>
      </div>
    </div>
  </div>
</template>



<style scoped>
.profile-card { padding: 0; overflow: hidden; }
.profile-top {
  padding: 28px 32px;
  display: flex;
  align-items: center;
  gap: 20px;
  background: linear-gradient(135deg, rgba(79,142,247,.05), rgba(124,90,247,.05));
}
.profile-avatar {
  width: 64px; height: 64px;
  border-radius: 50%;
  background: linear-gradient(135deg, var(--c-accent), var(--c-accent2));
  display: grid; place-items: center;
  font-family: var(--font-display);
  font-size: 1.4rem; font-weight: 700;
  color: #fff;
  flex-shrink: 0;
}
.profile-name { font-size: 1.2rem; margin-bottom: 6px; }
.profile-tags { display: flex; gap: 6px; }
.profile-form { padding: 24px 32px; display: flex; flex-direction: column; gap: 16px; }
.form-row { display: grid; grid-template-columns: 1fr 1fr; gap: 16px; }
.form-hint { font-size: .78rem; color: var(--c-text3); margin-top: 6px; }
.form-footer { display: flex; justify-content: flex-end; }

.stats-mini { margin-top: 16px; padding: 24px; }
.stats-mini h3 { font-size: .9rem; margin-bottom: 16px; color: var(--c-text2); }
.stats-mini-row { display: grid; grid-template-columns: repeat(4,1fr); gap: 16px; }
.stat-mini-item { text-align: center; }
.stat-mini-val { display: block; font-family: var(--font-display); font-size: 1.6rem; font-weight: 700; }
.stat-mini-lbl { font-size: .75rem; color: var(--c-text3); }
</style>